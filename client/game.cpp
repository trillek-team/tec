#include "game.hpp"

#include <devices/tda.hpp>
#include <default-config.hpp>

#include "controllers/fps-controller.hpp"
#include "server-message.hpp"
#include "events.hpp"
#include "event-system.hpp"
#include "graphics/view.hpp"
#include "graphics/texture-object.hpp"
#include "graphics/material.hpp"
#include "graphics/vertex-buffer-object.hpp"
#include "graphics/renderable.hpp"
#include "resources/pixel-buffer.hpp"
#include "components/lua-script.hpp"

namespace tec {
	using networking::ServerMessage;
	using networking::MessageType;

	Game::Game(OS &_os, std::string config_file_name) :
		os(_os),
		stats(),
		game_state_queue(this->stats),
		server_connection(this->stats),
		ps(this->simulation.GetPhysicsSystem()),
		vcs(this->simulation.GetVComputerSystem()),
		sound_thread([this] () { ss.Update(); }) {
		this->config_script = this->lua_sys.LoadFile(FilePath::GetAssetPath(config_file_name));
		this->server_connection.RegisterMessageHandler(
			MessageType::CLIENT_ID,
			[this] (const ServerMessage&) {
				auto client_id = server_connection.GetClientID();
				game_state_queue.SetClientID(client_id);

				this->player_camera = std::make_shared<FPSController>(client_id);
				Entity(client_id).Add<View>(true);
				auto cae_event = std::make_shared<ControllerAddedEvent>();
				cae_event->controller = this->player_camera;
				EventSystem<ControllerAddedEvent>::Get()->Emit(cae_event);
			});

		this->server_connection.RegisterConnectFunc(
			[this] () {
				asio_thread = new std::thread(
					[this] () {
						server_connection.StartRead();
					});
				sync_thread = new std::thread(
					[this] () {
						server_connection.StartSync();
					});
			});
	}

	void Game::Startup() {
		this->rs.Startup();
		const unsigned int window_width = this->config_script->environment.get_or("window_width", WINDOW_WIDTH);
		const unsigned int window_height = this->config_script->environment.get_or("window_height", WINDOW_HEIGHT);
		this->rs.SetViewportSize(window_width, window_height);
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

	void Game::UpdateVComputerScreenTextures() {
		tda::TDAScreen screen;
		static PixelBuffer local_pbuffer(320, 240, 8, ImageColorMode::COLOR_RGBA);

		for (auto computer_itr = ComputerComponentMap::Begin(); computer_itr != ComputerComponentMap::End(); ++computer_itr) {
			auto comp = *computer_itr;
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

	void Game::Update(double delta, double mouse_x, double mouse_y, int window_width, int window_height) {
		// TODO: a better representation of commands so we can send them less often
		const double COMMAND_RATE = 1.0 / 30.0;
		delta_accumulator += delta;
		game_state_queue.ProcessEventQueue();
		game_state_queue.Interpolate(delta);

		auto client_state = simulation.Simulate(delta, game_state_queue.GetInterpolatedState());
		game_state_queue.UpdatePredictions(client_state);

		while (delta_accumulator >= COMMAND_RATE) {
			if (this->player_camera) {
				ServerMessage update_message;
				proto::ClientCommands client_commands = this->player_camera->GetClientCommands();
				client_commands.set_commandid(command_id++);
				update_message.SetStateID(server_connection.GetLastRecvStateID());
				update_message.SetMessageType(MessageType::CLIENT_COMMAND);
				update_message.SetBodyLength(client_commands.ByteSizeLong());
				client_commands.SerializeToArray(update_message.GetBodyPTR(), static_cast<int>(update_message.GetBodyLength()));
				update_message.encode_header();
				server_connection.Send(update_message);
				game_state_queue.SetCommandID(command_id);
			}

			delta_accumulator -= COMMAND_RATE;
		}

		UpdateVComputerScreenTextures();
		ss.SetDelta(delta);
		rs.Update(delta, client_state);
		lua_sys.Update(delta);

		if (this->player_camera != nullptr) {
			if (this->player_camera->mouse_look) {
				os.EnableMouseLock(); // TODO: create event to change to mouse look
				this->active_entity = ps.RayCastMousePick(
					this->server_connection.GetClientID(),
					static_cast<float>(window_width) / 2.0f,
					static_cast<float>(window_height) / 2.0f,
					static_cast<float>(window_width),
					static_cast<float>(window_height)
				);
			}
			else {
				os.DisableMouseLock(); // TODO: create event to change from mouse look
				this->active_entity = ps.RayCastMousePick(
					this->server_connection.GetClientID(), mouse_x, mouse_y,
					static_cast<float>(window_width),
					static_cast<float>(window_height)
				);
			}
		}
	}
}
