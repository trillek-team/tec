#include "game.hpp"

#include <default-config.hpp>
#include <devices/tda.hpp>

#include "components/lua-script.hpp"
#include "controllers/fps-controller.hpp"
#include "engine-entities.hpp"
#include "event-system.hpp"
#include "events.hpp"
#include "graphics/material.hpp"
#include "graphics/renderable.hpp"
#include "graphics/texture-object.hpp"
#include "graphics/vertex-buffer-object.hpp"
#include "graphics/view.hpp"
#include "net-message.hpp"
#include "resources/pixel-buffer.hpp"

namespace tec {
using networking::MessageType;

Game::Game(OS& _os, std::string config_file_name) :
		stats(), os(_os), game_state_queue(this->stats), server_connection(this->stats),
		ps(this->simulation.GetPhysicsSystem()), vcs(this->simulation.GetVComputerSystem()),
		sound_thread([this]() { ss.Update(); }) {
	this->config_script = this->lua_sys.LoadFile(FilePath::GetAssetPath(config_file_name));
	this->server_connection.RegisterMessageHandler(MessageType::CLIENT_ID, [this](networking::MessageIn&) {
		auto client_id = server_connection.GetClientID();
		game_state_queue.SetClientID(client_id);

		this->player_camera = std::make_shared<FPSController>(client_id);
		Entity(client_id).Add<View>(true);
		this->player_entity_id = client_id;
		auto cae_event = std::make_shared<ControllerAddedEvent>();
		cae_event->controller = this->player_camera;
		EventSystem<ControllerAddedEvent>::Get()->Emit(cae_event);
	});

	asio_thread = new std::thread([this]() { server_connection.StartDispatch(); });

	this->server_connection.RegisterConnectFunc([this]() {
		if (this->sync_thread) {
			this->sync_thread->join();
			delete this->sync_thread;
		}
		sync_thread = new std::thread([this]() { server_connection.StartSync(); });
	});

	CreateEngineEntities();
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
	if (this->sync_thread) {
		this->sync_thread->join();
		delete this->sync_thread;
	}
}

void Game::Startup() {
	this->rs.Startup();
	const unsigned int window_width = this->config_script->environment.get_or("window_width", WINDOW_WIDTH);
	const unsigned int window_height = this->config_script->environment.get_or("window_height", WINDOW_HEIGHT);
	this->rs.SetViewportSize(window_width, window_height);
}

void Game::UpdateVComputerScreenTextures() {
	tda::TDAScreen screen;
	static PixelBuffer local_pbuffer(320, 240, 8, ImageColorMode::COLOR_RGBA);

	for (auto itr = ComputerComponentMap::Begin(); itr != ComputerComponentMap::End(); ++itr) {
		auto comp = *itr;
		std::shared_ptr<ComputerScreen> comp_screen = std::static_pointer_cast<ComputerScreen>(comp.second->devices[5]);
		std::static_pointer_cast<tda::TDADev>(comp_screen->device)->DumpScreen(screen);
		std::lock_guard<std::mutex> lock(local_pbuffer.GetWritelock());
		tda::TDAtoRGBATexture(screen, (std::uint32_t*)local_pbuffer.GetBlockBase());
		if (comp_screen->texture) {
			comp_screen->texture->Load(local_pbuffer);
		}
		else {
			Entity screen_entity(comp.first);
			if (screen_entity.Has<Renderable>()) {
				const auto* ren = screen_entity.Get<Renderable>();
				if (ren->buffer) {
					if (ren->buffer->GetVertexGroupCount() > 0) {
						auto texture_instance = std::make_shared<TextureObject>(local_pbuffer);
						auto material = Material::Create(std::to_string(comp.first) + "_screen");
						material->AddTexture(texture_instance);
						ren->buffer->GetVertexGroup(0)->material = material;
						comp_screen->texture = texture_instance;
					}
				}
			}
		}
	}
}

float Game::GetElapsedTime() {
	double new_time = os.GetTime();
	double elapsed_time = new_time - this->last_time;
	this->last_time = new_time;
	return static_cast<float>(elapsed_time);
}

void Game::Update(double delta, double mouse_x, double mouse_y, int window_width, int window_height) {
	this->ProcessEvents();
	// Elapsed time spend outside game loop
	tfm.outside_game_time = GetElapsedTime();

	// TODO: a better representation of commands so we can send them less often
	const double COMMAND_RATE = 1.0 / 30.0;
	delta_accumulator += delta;
	game_state_queue.ProcessEventQueue();
	game_state_queue.Interpolate(delta);

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
		if (this->player_camera->mouse_look) {
			os.EnableMouseLock(); // TODO: create event to change to mouse look
			this->active_entity = ps.RayCastMousePick(
					this->server_connection.GetClientID(),
					static_cast<float>(window_width) / 2.0f,
					static_cast<float>(window_height) / 2.0f,
					static_cast<float>(window_width),
					static_cast<float>(window_height));
			auto intersection = ps.GetLastRayPos();
			auto player_position = ps.GetPosition(player_entity_id);
			placement.SetRayIntersectionPoint(player_position.value, intersection);
		}
		else {
			os.DisableMouseLock(); // TODO: create event to change from mouse look
			this->active_entity = ps.RayCastMousePick(
					this->server_connection.GetClientID(),
					mouse_x,
					mouse_y,
					static_cast<float>(window_width),
					static_cast<float>(window_height));
		}
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
}

void Game::On(std::shared_ptr<KeyboardEvent> data) {
	if (data->action == KeyboardEvent::KEY_UP) {
		switch (data->key) {
		case GLFW_KEY_1: this->placement.SetMesh(MeshMap::Get(this->placeable_meshes[0])); break;
		case GLFW_KEY_2: this->placement.SetMesh(MeshMap::Get(this->placeable_meshes[1])); break;
		case GLFW_KEY_3: this->placement.SetMesh(MeshMap::Get(this->placeable_meshes[2])); break;
		case GLFW_KEY_4: this->placement.SetMesh(MeshMap::Get(this->placeable_meshes[3])); break;
		case GLFW_KEY_5: this->placement.SetMesh(MeshMap::Get(this->placeable_meshes[4])); break;
		case GLFW_KEY_6: this->placement.SetMesh(MeshMap::Get(this->placeable_meshes[5])); break;
		case GLFW_KEY_7: this->placement.SetMesh(MeshMap::Get(this->placeable_meshes[6])); break;
		case GLFW_KEY_8: this->placement.SetMesh(MeshMap::Get(this->placeable_meshes[7])); break;
		case GLFW_KEY_9: this->placement.SetMesh(MeshMap::Get(this->placeable_meshes[8])); break;
		case GLFW_KEY_0: this->placement.SetMesh(MeshMap::Get(this->placeable_meshes[9])); break;
		default: break;
		}
	}
}

void Game::On(std::shared_ptr<MouseClickEvent> data) {
	if (data->button == MouseBtnEvent::LEFT) {
		this->placement.PlaceEntityInWorld(data->ray_hit_point_world);
	}
}
} // namespace tec
