#include "game.hpp"

#include <default-config.hpp>
#include <devices/tda.hpp>

#include "components/lua-script.hpp"
#include "controllers/fps-controller.hpp"
#include "event-system.hpp"
#include "events.hpp"
#include "graphics/material.hpp"
#include "graphics/renderable.hpp"
#include "graphics/texture-object.hpp"
#include "graphics/vertex-buffer-object.hpp"
#include "graphics/view.hpp"
#include "gui/console.hpp"
#include "net-message.hpp"
#include "resources/pixel-buffer.hpp"

constexpr double COMMAND_RATE = 1.0 / 30.0;

namespace tec {
using networking::MessageType;

void CreateManipulatorEntity() {
	proto::Entity entity;
	entity.set_id(ENGINE_ENTITIES::MANIPULATOR);
	entity.add_components()->mutable_renderable();
	EventSystem<EntityCreated>::Get()->Emit(std::make_shared<EntityCreated>(EntityCreated{entity}));
}

void CreateEngineEntities() { CreateManipulatorEntity(); }

Game::Game(OS& _os, std::string config_file_name) :
		stats(), os(_os), game_state_queue(this->stats), server_connection(this->stats),
		ps(this->simulation.GetPhysicsSystem()), vcs(this->simulation.GetVComputerSystem()),
		sound_thread([this]() { ss.Update(); }) {
	this->config_script = this->lua_sys.LoadFile(Path::assets / config_file_name);
	this->server_connection.RegisterMessageHandler(MessageType::CLIENT_ID, [this](networking::MessageIn&) {
		auto client_id = server_connection.GetClientID();
		game_state_queue.SetClientID(client_id);

		this->player_camera = std::make_shared<FPSController>(client_id);
		Entity(client_id).Add<View>(true);
		this->player_entity_id = client_id;
		const auto cae_event = std::make_shared<ControllerAddedEvent>();
		cae_event->controller = this->player_camera;
		EventSystem<ControllerAddedEvent>::Get()->Emit(cae_event);
	});

	asio_thread = new std::thread([this]() { server_connection.StartDispatch(); });

	CreateEngineEntities();

	{
		auto& lua_state = this->lua_sys.GetGlobalState();
		lua_state["placement_manipulator"] = &this->placement;
	}
}

Game::~Game() {
	this->config_script.reset();
	this->ss.Stop();
	this->sound_thread.join();

	this->server_connection.Disconnect();
	this->server_connection.Stop();
	if (this->asio_thread) {
		this->asio_thread->join();
		delete this->asio_thread;
	}
}

void Game::Startup(Console& console) {
	this->rs.Startup();
	this->rs.RegisterConsole(console);
	const unsigned int window_width = this->config_script->environment.get_or("window_width", WINDOW_WIDTH);
	const unsigned int window_height = this->config_script->environment.get_or("window_height", WINDOW_HEIGHT);
	this->rs.SetViewportSize(glm::uvec2(window_width, window_height));
	this->placement.SetMaxDistance(this->config_script->environment.get_or(
			"max_placement_distance", manipulator::DEFAULT_MAX_PLACEMENT_DISTANCE));

	console.AddConsoleCommand("voxel_edit", "Voxel editing: voxel_edit {on|off}", [&](const std::string& command_args) {
		const auto argv = SplitString(command_args);
		if (argv.empty()) {
			return;
		}
		if (argv[0] == "on") {
			vox_sys.edit_allowed = true;
		}
		else if (argv[0] == "off") {
			vox_sys.edit_allowed = false;
		}
	});
}

void Game::UpdateVComputerScreenTextures() {
	tda::TDAScreen screen;
	static PixelBuffer local_pbuffer(320, 240, 8, ImageColorMode::COLOR_RGBA);

	for (auto itr = ComputerComponentMap::Begin(); itr != ComputerComponentMap::End(); ++itr) {
		const auto [entity_id, computer_component] = *itr;
		const std::shared_ptr<ComputerScreen> comp_screen =
				std::static_pointer_cast<ComputerScreen>(computer_component->devices[5]);
		std::static_pointer_cast<tda::TDADev>(comp_screen->device)->DumpScreen(screen);
		std::lock_guard lock(local_pbuffer.GetWritelock());
		TDAtoRGBATexture(screen, (std::uint32_t*)local_pbuffer.GetBlockBase());
		if (comp_screen->texture) {
			comp_screen->texture->Load(local_pbuffer);
		}
		else {
			if (Entity screen_entity(entity_id); screen_entity.Has<Renderable>()) {
				if (const auto* ren = screen_entity.Get<Renderable>();
					ren->render_item && !ren->render_item->vertex_groups.empty()) {
					const auto texture_instance = std::make_shared<TextureObject>(local_pbuffer);
					comp_screen->texture = texture_instance;
					const auto material = Material::Create(std::to_string(entity_id) + "_screen");
					material->AddTexture(texture_instance);
					ren->render_item->vertex_groups[0].material = material;
				}
			}
		}
	}
}

float Game::GetElapsedTime() {
	const double new_time = os.GetTime();
	const double elapsed_time = new_time - this->last_time;
	this->last_time = new_time;
	return static_cast<float>(elapsed_time);
}

void Game::Update(const double delta) {
	this->ProcessEvents();
	// Elapsed time spend outside game loop
	tfm.outside_game_time = GetElapsedTime();
	delta_accumulator += delta;
	game_state_queue.ProcessEventQueue();
	game_state_queue.Interpolate(delta);
	vox_sys.Update(delta);

	auto client_state = simulation.Simulate(delta, game_state_queue.GetInterpolatedState());
	game_state_queue.UpdatePredictions(client_state);

	while (delta_accumulator >= COMMAND_RATE) {
		if (this->player_camera) {
			networking::MessageOut update_message(MessageType::CLIENT_COMMAND);
			proto::ClientCommands client_commands = this->player_camera->GetClientCommands();
			client_commands.set_commandid(command_id++);
			client_commands.set_laststateid(server_connection.GetLastRecvStateID());
			client_commands.SerializeToZeroCopyStream(&update_message);
			server_connection.Send(std::move(update_message));
			game_state_queue.SetCommandID(command_id);
		}

		delta_accumulator -= COMMAND_RATE;
	}
	tfm.state_queue_time = GetElapsedTime();

	UpdateVComputerScreenTextures();
	tfm.vcomputer_time = GetElapsedTime();

	ss.SetDelta(delta);
	tfm.sound_system_time = GetElapsedTime();

	rs.Update(delta);
	tfm.render_system_time = GetElapsedTime();

	lua_sys.Update(delta);
	tfm.lua_system_time = GetElapsedTime();

	// Frames per second
	//
	// If one second has passed, we have the number of frames in that second, giving us frames per second
	frame_deltas += delta;
	frames++;
	if (frame_deltas >= 1) {
		fps = frames;
		avg_frame_time = static_cast<float>(frame_deltas / frames);
		frame_deltas = 0;
		frames = 0;
	}

	if (this->player_camera != nullptr) {
		const int window_width = os.GetWindowWidth();
		const int window_height = os.GetWindowHeight();
		double mouse_x, mouse_y;
		if (this->player_camera->mouse_look) {
			os.EnableMouseLock(); // TODO: create event to change to mouse look
			mouse_x = static_cast<double>(window_width) / 2.0;
			mouse_y = static_cast<double>(window_height) / 2.0;
			const auto intersection = ps.GetLastRayPos();
			const auto player_position = ps.GetPosition(player_entity_id);
			placement.SetRayIntersectionPoint(player_position.value, intersection);
		}
		else {
			os.DisableMouseLock(); // TODO: create event to change from mouse look
			OS::GetMousePosition(&mouse_x, &mouse_y);
		}
		this->active_entity = ps.RayCastMousePick(
				this->server_connection.GetClientID(),
				mouse_x,
				mouse_y,
				static_cast<float>(window_width),
				static_cast<float>(window_height));
	}
	tfm.other_time = GetElapsedTime();
	// clang-format off
	tfm.total_time = tfm.outside_game_time
				+ tfm.state_queue_time
				+ tfm.vcomputer_time
				+ tfm.sound_system_time
				+ tfm.render_system_time
				+ tfm.lua_system_time
				+ tfm.other_time;
	// clang-format on
}

void Game::ProcessEvents() {
	EventQueue<KeyboardEvent>::ProcessEventQueue();
	EventQueue<MouseClickEvent>::ProcessEventQueue();
	EventQueue<ChatCommandEvent>::ProcessEventQueue();
}

void Game::On(eid, std::shared_ptr<KeyboardEvent> data) {
	if (data->action == KeyboardEvent::KEY_UP) {
		this->lua_sys.CallFunctions("onKeyUp", data->key);
	}
}

void Game::On(eid, std::shared_ptr<MouseClickEvent> data) {
	if (data->button == MouseBtnEvent::LEFT) {
		this->placement.PlaceEntityInWorld(data->ray_hit_point_world);
	}
}

void Game::On(eid, std::shared_ptr<ChatCommandEvent> event) {
	if (event->command == "lua") {
		std::string lua_command = event->args.empty() ? "" : event->args[0];
		this->lua_sys.ExecuteString(lua_command);
	}
}

} // namespace tec
