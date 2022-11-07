#include "render-system.hpp"

#include <cmath>
#include <forward_list>
#include <thread>

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <spdlog/spdlog.h>

#include <google/protobuf/util/json_util.h>

#include "components/transforms.hpp"
#include "entity.hpp"
#include "events.hpp"
#include "graphics/animation.hpp"
#include "graphics/lights.hpp"
#include "graphics/material.hpp"
#include "graphics/renderable.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture-object.hpp"
#include "graphics/view.hpp"
#include "gui/console.hpp"
#include "multiton.hpp"
#include "proto-load.hpp"
#include "resources/mesh.hpp"
#include "resources/obj.hpp"
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

const GLSymbol& GLSymbol::Get(GLenum which) {
	static const std::map<GLenum, GLSymbol> symbolic_gl_types{
			// error codes
			{GL_NO_ERROR, {"GL_NO_ERROR"}},
			{GL_INVALID_ENUM, {"GL_INVALID_ENUM"}},
			{GL_INVALID_VALUE, {"GL_INVALID_VALUE"}},
			{GL_INVALID_OPERATION, {"GL_INVALID_OPERATION"}},
			{GL_INVALID_FRAMEBUFFER_OPERATION, {"GL_INVALID_FRAMEBUFFER_OPERATION"}},
			{GL_OUT_OF_MEMORY, {"GL_OUT_OF_MEMORY"}},
			// image formats
			{GL_DEPTH_COMPONENT, {"GL_DEPTH_COMPONENT"}},
			{GL_DEPTH_STENCIL, {"GL_DEPTH_STENCIL"}},
			{GL_RED, {"GL_RED"}},
			{GL_RG, {"GL_RG"}},
			{GL_RGB, {"GL_RGB"}},
			{GL_RGBA, {"GL_RGBA"}},
			{GL_SRGB, {"GL_SRGB"}},
			{GL_SRGB_ALPHA, {"GL_SRGB_ALPHA"}},
			{GL_SRGB8_ALPHA8, {"GL_SRGB8_ALPHA8"}},
			// shader types
			{GL_FLOAT, {"float"}},
			{GL_FLOAT_VEC2, {"vec2"}},
			{GL_FLOAT_VEC3, {"vec3"}},
			{GL_FLOAT_VEC4, {"vec4"}},
			{GL_INT, {"int"}},
			{GL_INT_VEC2, {"ivec2"}},
			{GL_INT_VEC3, {"ivec3"}},
			{GL_INT_VEC4, {"ivec4"}},
			{GL_SAMPLER_1D, {"sampler1D"}},
			{GL_SAMPLER_2D, {"sampler2D"}},
			{GL_SAMPLER_3D, {"sampler3D"}},
			{GL_SAMPLER_2D_SHADOW, {"sampler2DShadow"}},
			{GL_SAMPLER_CUBE_SHADOW, {"samplerCubeShadow"}},
			{GL_SAMPLER_2D_ARRAY, {"sampler2DArray"}},
			{GL_UNSIGNED_INT, {"uint"}},
			{GL_FLOAT_MAT4, {"mat4x4"}},
	};
	static std::forward_list<std::string> unknown_gl_strings;
	static std::map<GLenum, GLSymbol> unknown_gl_types;
	if (const auto sym_itr = symbolic_gl_types.find(which); sym_itr != symbolic_gl_types.cend()) {
		return sym_itr->second;
	}
	unknown_gl_strings.push_front(std::to_string(which));
	return unknown_gl_types.emplace(std::make_pair(which, GLSymbol{unknown_gl_strings.front()})).first->second;
}

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
			_log->error("[RenderSystem] Config shader_set has unknown condition: {}", shader_set.condition_case());
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
	if (auto sphere = OBJ::Create(Path("assets:/sphere/sphere.obj")); !sphere) {
		_log->debug("[RenderSystem] Error loading sphere.obj.");
	}
	else {
		this->sphere_vbo.Load(sphere);
	}
	if (auto quad = OBJ::Create(Path("assets:/quad/quad.obj")); !quad) {
		_log->debug("[RenderSystem] Error loading quad.obj.");
	}
	else {
		this->quad_vbo.Load(quad);
	}

	this->inv_view_size.x = 1.0f / static_cast<float>(this->view_size.x);
	this->inv_view_size.y = 1.0f / static_cast<float>(this->view_size.y);
	this->light_gbuffer.AddColorAttachments(this->view_size.x, this->view_size.y);
	this->light_gbuffer.SetDepthAttachment(GBuffer::DEPTH_TYPE::DEPTH, this->view_size.x, this->view_size.y);
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

void RenderSystem::SetViewportSize(const glm::uvec2& _view_size) {
	const auto viewport = glm::max(glm::uvec2(1), _view_size);
	this->view_size = viewport;
	this->inv_view_size = 1.0f / glm::vec2(viewport);
	float aspect_ratio = static_cast<float>(viewport.x) / static_cast<float>(viewport.y);
	if ((aspect_ratio < 1.0f) || std::isnan(aspect_ratio)) {
		aspect_ratio = 4.0f / 3.0f;
	}

	this->projection = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 10000.0f);
	// convert the projection to reverse depth with infinite far plane
	this->projection[2][2] = 0.0f;
	this->projection[3][2] = 0.1f;
	this->light_gbuffer.ResizeColorAttachments(viewport.x, viewport.y);
	this->light_gbuffer.ResizeDepthAttachment(viewport.x, viewport.y);
	glViewport(0, 0, viewport.x, viewport.y);
}

void RenderSystem::ErrorCheck(const std::string_view what, size_t line, const std::string_view where) {
	if (const GLenum err = glGetError()) {
		_log->log(spdlog::level::debug, "[{}:{}] {}. Error={}", where, line, what, GLSymbol::Get(err));
	}
}

void RenderSystem::Update(const double delta) {
	ProcessCommandQueue();
	EventQueue<WindowResizedEvent>::ProcessEventQueue();
	EventQueue<EntityCreated>::ProcessEventQueue();
	EventQueue<EntityDestroyed>::ProcessEventQueue();

	ErrorCheck("Preframe", __LINE__);
	UpdateRenderList(delta);
	this->light_gbuffer.StartFrame();
	ErrorCheck("StartFrame", __LINE__);

	GeometryPass();
	ErrorCheck("GeometryPass", __LINE__);

	this->light_gbuffer.BeginLightPass();
	BeginPointLightPass();
	DirectionalLightPass();

	ErrorCheck("*LightPass", __LINE__);

	FinalPass();
	if (options.debug_gbuffer()) {
		RenderGbuffer();
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

void RenderSystem::GeometryPass() {
	this->light_gbuffer.BeginGeometryPass();

	glm::vec3 camera_pos;
	glm::quat camera_quat;
	{
		if (const View* view = this->current_view) {
			camera_pos = view->view_pos;
			camera_quat = view->view_quat;
		}
	}

	const GLuint def_textures[]{TextureMap::Get("default")->GetID(), TextureMap::Get("default_sp")->GetID()};
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, def_textures[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, def_textures[1]);
	const std::shared_ptr<Shader> def_shader = ShaderMap::Get(active_shaders.visual());
	def_shader->Use();
	glUniform3fv(def_shader->GetUniformLocation("view_pos"), 1, glm::value_ptr(camera_pos));
	glUniform4fv(def_shader->GetUniformLocation("view_quat"), 1, glm::value_ptr(camera_quat));
	glUniformMatrix4fv(def_shader->GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(this->projection));
	struct uniform_locs {
		GLint model_pos;
		GLint model_scale;
		GLint model_quat;
		GLint bones;
		GLint animated;
		GLint vertex_group;
	};
	uniform_locs def_locs{
			def_shader->GetUniformLocation("model_position"),
			def_shader->GetUniformLocation("model_scale"),
			def_shader->GetUniformLocation("model_quat"),
			def_shader->GetUniformLocation("animated"),
			def_shader->GetUniformLocation("animation_matrix"),
			-1};
	uniform_locs spec_locs{0, 0, 0, 0, 0};
	const uniform_locs* use_locs = &def_locs;
	for (auto [_shader, _render_item] : this->render_item_list) {
		// Check if we need to use a specific shader and set it up.
		if (_shader) {
			def_shader->UnUse();
			_shader->Use();
			glUniform3fv(_shader->GetUniformLocation("view_pos"), 1, glm::value_ptr(camera_pos));
			glUniform4fv(_shader->GetUniformLocation("view_quat"), 1, glm::value_ptr(camera_quat));
			glUniformMatrix4fv(
					_shader->GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(this->projection));
			spec_locs.model_pos = _shader->GetUniformLocation("model_position");
			spec_locs.model_scale = _shader->GetUniformLocation("model_scale");
			spec_locs.model_quat = _shader->GetUniformLocation("model_quat");
			spec_locs.bones = _shader->GetUniformLocation("animation_bones[0]");
			spec_locs.animated = _shader->GetUniformLocation("animated");
			spec_locs.vertex_group = _shader->GetUniformLocation("vertex_group");
			use_locs = &spec_locs;
		}
		for (const auto render_item : _render_item) {
			glBindVertexArray(render_item->vbo->GetVAO());
			glUniform1i(use_locs->animated, render_item->animated ? 1 : 0);

			if (render_item->animated && use_locs->bones != -1) {
				auto& bones = render_item->animation->bone_transforms;
				glUniformMatrix3x4fv(use_locs->bones, bones.size(), GL_FALSE, glm::value_ptr(bones[0].orientation));
			}
			GLint vertex_group_number = 0;
			for (auto& [material, mesh_group_number, index_count, starting_offset] : render_item->vertex_groups) {
				glPolygonMode(GL_FRONT_AND_BACK, material->GetPolygonMode());
				ActivateMaterial(*material);
				if (use_locs->vertex_group != -1) {
					glUniform1i(use_locs->vertex_group, vertex_group_number++);
				}
				//glUniformMatrix4fv(model_index, 1, GL_FALSE, glm::value_ptr(render_item->model_matrix));
				glUniform3fv(use_locs->model_pos, 1, glm::value_ptr(render_item->model_position));
				glUniform3fv(use_locs->model_scale, 1, glm::value_ptr(render_item->model_scale));
				glUniform4fv(use_locs->model_quat, 1, glm::value_ptr(render_item->model_quat));
				glDrawElements(
						material->GetDrawElementsMode(),
						static_cast<GLsizei>(index_count),
						GL_UNSIGNED_INT,
						(GLvoid*)(starting_offset * sizeof(GLuint)));
				DeactivateMaterial(*material, def_textures);
			}
		}
		// If we used a special shader set things back to the deferred shader.
		if (_shader) {
			_shader->UnUse();
			def_shader->Use();
			use_locs = &def_locs;
		}
	}
	def_shader->UnUse();
	glBindVertexArray(0);
	this->light_gbuffer.EndGeometryPass();
}

void RenderSystem::BeginPointLightPass() {
	glm::vec3 camera_pos;
	glm::quat camera_quat;
	{
		if (const View* view = this->current_view) {
			camera_pos = view->view_pos;
			camera_quat = view->view_quat;
		}
	}

	const std::shared_ptr<Shader> def_pl_shader = ShaderMap::Get(active_shaders.pointlight());
	def_pl_shader->Use();
	glUniform3fv(def_pl_shader->GetUniformLocation("view_pos"), 1, glm::value_ptr(camera_pos));
	glUniform4fv(def_pl_shader->GetUniformLocation("view_quat"), 1, glm::value_ptr(camera_quat));
	glUniformMatrix4fv(def_pl_shader->GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(this->projection));
	glUniform2f(def_pl_shader->GetUniformLocation("gScreenSize"), this->inv_view_size.x, this->inv_view_size.y);
	const GLint model_index = def_pl_shader->GetUniformLocation("model");
	const GLint color_index = def_pl_shader->GetUniformLocation("gPointLight.Base.Color");
	const GLint ambient_intensity_index = def_pl_shader->GetUniformLocation("gPointLight.Base.AmbientIntensity");
	const GLint diffuse_intensity_index = def_pl_shader->GetUniformLocation("gPointLight.Base.DiffuseIntensity");
	const GLint atten_constant_index = def_pl_shader->GetUniformLocation("gPointLight.Atten.Constant");
	const GLint atten_linear_index = def_pl_shader->GetUniformLocation("gPointLight.Atten.Linear");
	const GLint atten_exp_index = def_pl_shader->GetUniformLocation("gPointLight.Atten.Exp");

	glBindVertexArray(this->sphere_vbo.GetVAO());

	const auto index_count{static_cast<GLsizei>(this->sphere_vbo.GetVertexGroup(0)->index_count)};

	this->light_gbuffer.BeginLightPass();
	this->light_gbuffer.BeginPointLightPass();
	def_pl_shader->Use();

	for (auto itr = PointLightMap::Begin(); itr != PointLightMap::End(); ++itr) {
		const eid entity_id = itr->first;
		PointLight* light = itr->second;

		glm::vec3 position;
		if (Multiton<eid, Position*>::Has(entity_id)) {
			position = Multiton<eid, Position*>::Get(entity_id)->value;
		}

		light->UpdateBoundingRadius();
		glm::mat4 transform_matrix =
				glm::scale(glm::translate(glm::mat4(1.0), position), glm::vec3(light->bounding_radius));

		glUniformMatrix4fv(model_index, 1, GL_FALSE, glm::value_ptr(transform_matrix));
		glUniform3f(color_index, light->color.x, light->color.y, light->color.z);
		glUniform1f(ambient_intensity_index, light->ambient_intensity);
		glUniform1f(diffuse_intensity_index, light->diffuse_intensity);
		glUniform1f(atten_constant_index, light->Attenuation.constant);
		glUniform1f(atten_linear_index, light->Attenuation.linear);
		glUniform1f(atten_exp_index, light->Attenuation.exponential);
		glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);
	}
	// these would go in the loop for stencil lights
	this->light_gbuffer.EndPointLightPass();
	def_pl_shader->UnUse();

	glBindVertexArray(0);
}

void RenderSystem::DirectionalLightPass() {
	this->light_gbuffer.BeginDirLightPass();
	const std::shared_ptr<Shader> def_dl_shader = ShaderMap::Get(active_shaders.dirlight());
	def_dl_shader->Use();

	glm::vec3 camera_pos;
	{
		if (const View* view = this->current_view) {
			camera_pos = view->view_pos;
		}
	}
	glUniform2f(def_dl_shader->GetUniformLocation("gScreenSize"), this->inv_view_size.x, this->inv_view_size.y);
	glUniform3fv(def_dl_shader->GetUniformLocation("view_pos"), 1, glm::value_ptr(camera_pos));
	const GLint Color_index = def_dl_shader->GetUniformLocation("gDirectionalLight.Base.Color");
	const GLint AmbientIntensity_index = def_dl_shader->GetUniformLocation("gDirectionalLight.Base.AmbientIntensity");
	const GLint DiffuseIntensity_index = def_dl_shader->GetUniformLocation("gDirectionalLight.Base.DiffuseIntensity");
	const GLint direction_index = def_dl_shader->GetUniformLocation("gDirectionalLight.Direction");

	glBindVertexArray(this->quad_vbo.GetVAO());

	const auto index_count{static_cast<GLsizei>(this->quad_vbo.GetVertexGroup(0)->index_count)};

	for (auto itr = DirectionalLightMap::Begin(); itr != DirectionalLightMap::End(); ++itr) {
		const DirectionalLight* light = itr->second;

		glUniform3f(Color_index, light->color.x, light->color.y, light->color.z);
		glUniform1f(AmbientIntensity_index, light->ambient_intensity);
		glUniform1f(DiffuseIntensity_index, light->diffuse_intensity);
		glUniform3f(direction_index, light->direction.x, light->direction.y, light->direction.z);

		glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);
	}
	def_dl_shader->UnUse();
	glBindVertexArray(0);
}

void RenderSystem::FinalPass() {
	this->light_gbuffer.FinalPass();
	const std::shared_ptr<Shader> post_shader = ShaderMap::Get(active_shaders.postprocess());
	post_shader->Use();
	glUniform2f(post_shader->GetUniformLocation("gScreenSize"), this->inv_view_size.x, this->inv_view_size.y);

	glBindVertexArray(this->quad_vbo.GetVAO());
	const auto index_count{static_cast<GLsizei>(this->quad_vbo.GetVertexGroup(0)->index_count)};
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	post_shader->UnUse();
}

void RenderSystem::RenderGbuffer() {
	this->light_gbuffer.BindForRendering();
	glDisable(GL_BLEND);
	glActiveTexture(GL_TEXTURE0 + static_cast<int>(GBuffer::DEPTH_TYPE::DEPTH));
	glBindSampler(static_cast<int>(GBuffer::DEPTH_TYPE::DEPTH), 0);
	glBindTexture(GL_TEXTURE_2D, this->light_gbuffer.GetDepthTexture());
	glDrawBuffer(GL_BACK);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBindVertexArray(this->quad_vbo.GetVAO());

	const std::shared_ptr<Shader> def_db_shader = ShaderMap::Get(active_shaders.gbufdebug());
	def_db_shader->Use();

	glUniform2f(def_db_shader->GetUniformLocation("gScreenSize"), this->inv_view_size.x, this->inv_view_size.y);

	const auto index_count{static_cast<GLsizei>(this->quad_vbo.GetVertexGroup(0)->index_count)};
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);

	def_db_shader->UnUse();
	glBindVertexArray(0);
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

void RenderSystem::On(const eid entity_id, std::shared_ptr<EntityDestroyed> data) { RenderableMap::Remove(entity_id); }

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

void RenderSystem::UpdateRenderList(double delta) {
	this->render_item_list.clear();

	if (!this->default_shader) {
		this->default_shader = ShaderMap::Get("debug");
	}

	// Loop through each renderable and update its model matrix.
	for (auto itr = RenderableMap::Begin(); itr != RenderableMap::End(); ++itr) {
		eid entity_id = itr->first;
		Renderable* renderable = itr->second;
		if (renderable->hidden) {
			continue;
		}
		Entity entity(entity_id);
		auto [_position, _orientation, _scale, _animation] = entity.GetList<Position, Orientation, Scale, Animation>();
		glm::vec3 position = renderable->local_translation;
		if (_position) {
			position += _position->value;
		}
		glm::quat orientation = renderable->local_orientation.value;
		if (_orientation) {
			orientation *= _orientation->value;
		}
		glm::vec3 scale(1.0);
		if (_scale) {
			scale = _scale->value;
		}

		auto& mesh = renderable->mesh;
		auto& ri = renderable->render_item;
		if (!mesh && ri) {
			renderable->render_item.reset();
			ri.reset();
		}
		if (mesh && (!ri || ri->mesh_at_set != mesh.get())) {
			auto buffer_itr = mesh_buffers.find(mesh);
			std::shared_ptr<VertexBufferObject> buffer;
			if (buffer_itr == mesh_buffers.cend()) {
				buffer = std::make_shared<VertexBufferObject>(vertex::VF_FULL);
				buffer->Load(mesh);
				mesh_buffers[mesh] = buffer;
			}
			else {
				buffer = buffer_itr->second;
			}
			std::size_t group_count = buffer->GetVertexGroupCount();
			if (group_count > 0) {
				if (!ri) {
					ri = std::make_shared<RenderItem>();
				}
				else {
					ri->vertex_groups.clear();
				}
				ri->vbo = buffer;
				ri->vertex_groups.reserve(group_count);
				ri->mesh_at_set = mesh.get();
				for (std::size_t i = 0; i < group_count; ++i) {
					ri->vertex_groups.push_back(*buffer->GetVertexGroup(i));
				}
				renderable->render_item = ri;
			}
			else {
				_log->warn("[RenderSystem] empty mesh on Renderable [{}]", entity_id);
				renderable->render_item.reset();
				ri.reset();
			}
		}

		if (ri) {
			if (ri->vbo->Update()) {
				// the mesh updated it's contents
				std::size_t group_count = ri->vbo->GetVertexGroupCount();
				ri->vertex_groups.clear();
				ri->vertex_groups.reserve(group_count);
				for (std::size_t i = 0; i < group_count; ++i) {
					ri->vertex_groups.push_back(*ri->vbo->GetVertexGroup(i));
				}
			}
			ri->model_position = position;
			ri->model_scale = scale;
			ri->model_quat = orientation;

			if (_animation) {
				const_cast<Animation*>(_animation)->UpdateAnimation(delta);
				if (!_animation->bone_transforms.empty()) {
					ri->animated = true;
					ri->animation = const_cast<Animation*>(_animation);
				}
			}
			this->render_item_list[renderable->shader].insert(ri.get());
		}
	}

	for (auto itr = Multiton<eid, View*>::Begin(); itr != Multiton<eid, View*>::End(); ++itr) {
		eid entity_id = itr->first;
		View* view = itr->second;

		Entity entity(entity_id);
		auto [_position, _orientation] = entity.GetList<Position, Orientation>();
		glm::vec3 position;
		if (_position) {
			position = _position->value;
		}
		glm::quat orientation;
		if (_orientation) {
			orientation = _orientation->value;
		}

		view->view_pos = -position;
		view->view_quat = glm::inverse(orientation);
		if (view->active) {
			this->current_view = view;
		}
	}
}
} // namespace tec
