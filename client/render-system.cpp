#include "render-system.hpp"

#include <cmath>
#include <forward_list>
#include <thread>

#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>
#include <spdlog/spdlog.h>

#include <google/protobuf/util/json_util.h>

#include "components/transforms.hpp"
#include "events.hpp"
#include "graphics/animation.hpp"
#include "graphics/gl-symbol.hpp"
#include "graphics/lights.hpp"
#include "graphics/material.hpp"
#include "graphics/passes/debug-pass.hpp"
#include "graphics/passes/dir-light-pass.hpp"
#include "graphics/passes/final-pass.hpp"
#include "graphics/passes/geometry-pass.hpp"
#include "graphics/passes/point-light-pass.hpp"
#include "graphics/render-list.hpp"
#include "graphics/renderable.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture-object.hpp"
#include "graphics/view.hpp"
#include "gui/console.hpp"
#include "multiton.hpp"
#include "proto-load.hpp"
#include "resources/pixel-buffer.hpp"

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

namespace tec {
using PointLightMap = Multiton<eid, PointLight*>;
using DirectionalLightMap = Multiton<eid, DirectionalLight*>;
using RenderableMap = Multiton<eid, Renderable*>;
using AnimationMap = Multiton<eid, Animation*>;
using ScaleMap = Multiton<eid, Scale*>;

std::shared_ptr<spdlog::logger> RenderSystem::_log;

void RenderSystem::Startup() {
	_log = spdlog::get("console_log");

	// If there is an error that means something went wrong when creating the context.
	ErrorCheck("Something went wrong when creating the context", __LINE__);

	tec::gfx::RenderConfig render_config;
	auto core_fname = Path::shaders / "core.json";
	auto core_json = LoadAsString(core_fname);
	if (auto status = google::protobuf::util::JsonStringToMessage(core_json, &render_config); !status.ok()) {
		_log->error("[RenderSystem] loading config file: {} failed: {}", core_fname, status.ToString());
		return;
	}
	options.MergeFrom(render_config.options());
	active_shaders.set_visual("deferred");
	active_shaders.set_shadow("deferred_shadow");
	active_shaders.set_pointlight("deferred_pointlight");
	active_shaders.set_dirlight("deferred_dirlight");
	active_shaders.set_postprocess("post_process");
	active_shaders.set_gbufdebug("deferred_debug");
	for (auto& shader_set : options.shader_sets()) {
		switch (shader_set.condition_case()) {
		case gfx::ShaderSet::kDefault: active_shaders.MergeFrom(shader_set); break;
		case gfx::ShaderSet::CONDITION_NOT_SET: break;
		default:
			_log->error("[RenderSystem] Config shader_set has unknown condition: {}", (int)shader_set.condition_case());
			break;
		}
	}
	_log->debug("[RenderSystem] set visual={}", active_shaders.visual());
	_log->debug("[RenderSystem] set shadow={}", active_shaders.shadow());
	_log->debug("[RenderSystem] set pointlight={}", active_shaders.pointlight());
	_log->debug("[RenderSystem] set dirlight={}", active_shaders.dirlight());
	_log->debug("[RenderSystem] set postprocess={}", active_shaders.postprocess());
	_log->debug("[RenderSystem] set gbufdebug={}", active_shaders.gbufdebug());

	// load the list of extensions
	GLint num_extensions = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);
	std::string ext;
	for (GLint e = 0; e < num_extensions; e++) {
		extensions.emplace(std::string(reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, e))));
	}

#ifdef GL_ARB_clip_control
	if (HasExtension("GL_ARB_clip_control")) {
		_log->debug("[RenderSystem] Using glClipControl.");
		glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
	}
#endif
	// Black is the safest clear color since this is a space game.
	glClearColor(0.0, 0.0, 0.0, 0.0);
	// Reversed Z buffering for improved precision (maybe)
	glClearDepth(0.0);
	glDepthFunc(GL_GREATER);

	if (const auto quad = OBJ::Create(Path("assets:/quad/quad.obj")); !quad) {
		_log->debug("[RenderSystem] Error loading quad.obj.");
	}
	else {
		quad_vbo = std::make_unique<VertexBufferObject>(vertex::VF_BASE);
		quad_vbo->Load(quad);
	}

	this->render_passes.push_back(std::make_shared<graphics::pass::GeometryPass>());
	this->render_passes.push_back(std::make_shared<graphics::pass::PointLightPass>());
	this->render_passes.push_back(std::make_shared<graphics::pass::DirLightPass>());
	this->render_passes.push_back(std::make_shared<graphics::pass::FinalPass>());

	if (options.debug_gbuffer()) {
		this->render_passes.push_back(std::make_shared<graphics::pass::DebugPass>());
	}

	UpdateViewport({1024, 768});
	this->light_gbuffer.AddColorAttachments(viewport.view_size.x, viewport.view_size.y);
	this->light_gbuffer.SetDepthAttachment(GBuffer::DEPTH_TYPE::DEPTH, viewport.view_size.x, viewport.view_size.y);
	if (!this->light_gbuffer.CheckCompletion()) {
		_log->error("[RenderSystem] Failed to create Light GBuffer.");
	}

	constexpr uint32_t checker_size = 64;
	auto default_pbuffer = std::make_shared<PixelBuffer>(checker_size, checker_size, 8, ImageColorMode::COLOR_RGBA);
	{
		std::lock_guard lg(default_pbuffer->GetWritelock());
		auto pixels = reinterpret_cast<uint32_t*>(default_pbuffer->GetPtr());
		for (size_t y = 0; y < checker_size; y++) {
			for (size_t x = 0; x < checker_size; x++) {
				uint32_t c = ((x / 8) ^ (y / 8)) & 1; // c is 1 or 0 in an 8x8 checker pattern
				*(pixels++) = 0xff000000 | (c * 0xffffff); // set pixel with full alpha
			}
		}
	}
	PixelBufferMap::Set("default", default_pbuffer);
	auto default_texture = std::make_shared<TextureObject>(default_pbuffer);
	TextureMap::Set("default", default_texture);

	// SP controls metallic/roughness/etc
	auto default_sp_pbuffer = std::make_shared<PixelBuffer>(checker_size, checker_size, 8, ImageColorMode::COLOR_RGBA);
	{
		std::lock_guard lg(default_sp_pbuffer->GetWritelock());
		auto pixels = reinterpret_cast<uint32_t*>(default_sp_pbuffer->GetPtr());
		for (size_t y = 0; y < checker_size; y++) {
			for (size_t x = 0; x < checker_size; x++) {
				*(pixels++) = 0x00008000; // non-metallic, 50% roughness
			}
		}
	}
	PixelBufferMap::Set("default_sp", default_sp_pbuffer);
	auto default_sp_texture = std::make_shared<TextureObject>(default_sp_pbuffer);
	TextureMap::Set("default_sp", default_sp_texture);

	auto debug_fill = Material::Create("material_debug");
	debug_fill->SetPolygonMode(GL_LINE);
	debug_fill->SetDrawElementsMode(GL_LINES);

	SetupDefaultShaders(render_config);
	_log->info("[RenderSystem] Startup complete.");
}

void RenderSystem::RegisterConsole(Console& console) {
	console.AddConsoleCommand("rs_debug", "Toggle GBuffer debug render", [this](const std::string&) {
		this->options.set_debug_gbuffer(!this->options.debug_gbuffer());
	});
	console.AddConsoleCommand("rs_anim", "Control animation: {entity} (play|stop)", [](const std::string& args) {
		const auto argv = SplitString(args);
		eid entity_id = std::stoll(argv[0]);
		if (!AnimationMap::Has(entity_id)) {
			_log->error("[RSCommand] No such entity:{}", entity_id);
			return;
		}
		const auto anim = AnimationMap::Get(entity_id);
		if (!anim) {
			_log->error("[RSCommand] Bad animation:{}", entity_id);
		}
		if (argv[1] == "play") {
			anim->Play(false);
		}
		else if (argv[1] == "stop") {
			anim->Stop();
		}
	});
}

void RenderSystem::UpdateViewport(const glm::uvec2& view_size) {
	viewport.view_size = glm::max(glm::uvec2(1), view_size);
	viewport.inv_view_size = 1.0f / glm::vec2(viewport.view_size);
	float aspect_ratio = static_cast<float>(viewport.view_size.x) / static_cast<float>(viewport.view_size.y);
	if ((aspect_ratio < 1.0f) || std::isnan(aspect_ratio)) {
		aspect_ratio = 4.0f / 3.0f;
	}

	viewport.projection = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 10000.0f);
	// convert the projection to reverse depth with infinite far plane
	viewport.projection[2][2] = 0.0f;
	viewport.projection[3][2] = 0.1f;
}

void RenderSystem::SetViewportSize(const glm::uvec2& view_size) {
	this->UpdateViewport(view_size);
	this->light_gbuffer.ResizeColorAttachments(viewport.view_size.x, viewport.view_size.y);
	this->light_gbuffer.ResizeDepthAttachment(viewport.view_size.x, viewport.view_size.y);
	glViewport(0, 0, viewport.view_size.x, viewport.view_size.y);
}

void RenderSystem::ErrorCheck(const std::string_view what, size_t line, const std::string_view where) {
	if (const GLenum err = glGetError()) {
		_log->log(spdlog::level::debug, "[{}:{}] {}. Error={}", where, line, what, graphics::GLSymbol::Get(err));
	}
}

void RenderSystem::Update(const double delta) {
	ProcessCommandQueue();
	EventQueue<WindowResizedEvent>::ProcessEventQueue();
	EventQueue<EntityCreated>::ProcessEventQueue();
	EventQueue<EntityDestroyed>::ProcessEventQueue();

	ErrorCheck("Preframe", __LINE__);
	this->render_item_list.UpdateRenderList(delta);
	this->light_gbuffer.StartFrame();
	ErrorCheck("StartFrame", __LINE__);

	const auto current_view = this->render_item_list.GetCurrentView().value_or(View{true});
	const auto& render_items = this->render_item_list.GetRenderItems();

	for (const auto& pass : this->render_passes) {
		pass->Prepare(this->light_gbuffer);
		pass->Run(this->active_shaders, viewport, current_view, render_items);
		pass->Complete(this->light_gbuffer);
		ErrorCheck(pass->GetPassName(), __LINE__);
	}

	ErrorCheck("Postframe", __LINE__);
}

void ActivateTextureUnit(const GLuint unit, const GLuint texture_name) {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture_name);
}

void RenderSystem::ActivateMaterial(const Material& material) {
	for (GLuint unit = 0; unit < material.textures.size(); ++unit) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, material.textures[unit]->GetID());
	}
}

void RenderSystem::DeactivateMaterial(const Material& material, const GLuint* def_names) {
	for (GLuint unit = 0; unit < material.textures.size(); ++unit) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, (unit == 1) ? def_names[1] : def_names[0]);
	}
}

void RenderSystem::SetupDefaultShaders(const gfx::RenderConfig& render_config) {
	for (auto& shader_inc : render_config.includes()) {
		Shader::IncludeFromDef(shader_inc);
	}
	for (auto& shader_def : render_config.shaders()) {
		Shader::CreateFromDef(shader_def);
	}
}

void RenderSystem::On(eid, std::shared_ptr<WindowResizedEvent> data) {
	const glm::ivec2 new_view(data->new_width, data->new_height);
	SetViewportSize(glm::max(glm::ivec2(0), new_view));
}

void RenderSystem::On(const eid entity_id, std::shared_ptr<EntityDestroyed> data) {
	RenderableMap::Remove(entity_id);
	PointLightMap::Remove(entity_id);
	DirectionalLightMap::Remove(entity_id);
	AnimationMap::Remove(entity_id);
	ScaleMap::Remove(entity_id);
}

void RenderSystem::On(eid, const std::shared_ptr<EntityCreated> data) {
	const eid entity_id = data->entity.id();
	for (int i = 0; i < data->entity.components_size(); ++i) {
		switch (const proto::Component& comp = data->entity.components(i); comp.component_case()) {
		case proto::Component::kRenderable:
		{
			const auto renderable = new Renderable();
			renderable->In(comp);
			RenderableMap::Set(entity_id, renderable);
			break;
		}
		case proto::Component::kPointLight:
		{
			auto* point_light = new PointLight();
			point_light->In(comp);
			PointLightMap::Set(entity_id, point_light);
			break;
		}
		case proto::Component::kDirectionalLight:
		{
			auto* dir_light = new DirectionalLight();
			dir_light->In(comp);
			DirectionalLightMap::Set(entity_id, dir_light);
			break;
		}
		case proto::Component::kAnimation:
		{
			auto* animation = new Animation();
			animation->In(comp);
			AnimationMap::Set(entity_id, animation);
			break;
		}
		case proto::Component::kScale:
		{
			auto* scale = new Scale();
			scale->In(comp);
			ScaleMap::Set(entity_id, scale);
			break;
		}
		default: break;
		}
	}
}
} // namespace tec
