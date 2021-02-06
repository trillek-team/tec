


#include "vcomputer-system.hpp"

#include <GLFW/glfw3.h> // TODO: included for key constants

#include <vcomputer.hpp>
#include <auxiliar.hpp>
#include <tr3200/tr3200.hpp>
#include <dcpu16n/dcpu16n.hpp>
#include <devices/tda.hpp>
#include <devices/gkeyb.hpp>

#include "filesystem.hpp"
#include "entity.hpp"
#include "events.hpp"

// #include "client/graphics/texture-object.hpp"
// #include "client/graphics/material.hpp"
// #include "client/graphics/vertex-buffer-object.hpp"
// #include "client/graphics/renderable.hpp"

namespace tec {
	using namespace trillek::computer;

	Computer::Computer() : rom(new std::uint8_t[32 * 1024]), rom_size(0) {}

	void Computer::In(const proto::Component& source) {
		const proto::Computer& comp = source.computer();
		const proto::Computer::CPU& cpu = comp.cpu();
		switch (cpu.cpu_case()) {
			case proto::Computer::CPU::kTr3200:
			{
				const proto::Computer::CPU::TR3200& tr3200 = cpu.tr3200();
				TR3200State state;
				for (int i = 0; i < tr3200.registers_size(); ++i) {
					state.r[i] = tr3200.registers(i);
				}
				state.pc = tr3200.pc();
				state.wait_cycles = tr3200.wait_cycles();
				state.int_msg = static_cast<trillek::Word>(tr3200.int_msg());
				state.interrupt = tr3200.interrupt();
				state.step_mode = tr3200.step_mode();
				state.skiping = tr3200.skiping();
				state.sleeping = tr3200.sleeping();
				std::unique_ptr <TR3200> trcpu = std::make_unique<TR3200>();
				this->vc.SetCPU(std::move(trcpu));
				this->vc.On();
				this->vc.SetState(&state, sizeof(TR3200State));
			}
			break;
			case proto::Computer::CPU::kDcpu16N:
			{
				const proto::Computer::CPU::DCPU16N& dcpu16n = cpu.dcpu16n();
				DCPU16NState state;
				for (int i = 0; i < dcpu16n.registers_size(); ++i) {
					state.r[i] = dcpu16n.registers(i);
				}
				state.pc = dcpu16n.pc();
				state.wait_cycles = dcpu16n.wait_cycles();
				std::unique_ptr <DCPU16N> dcpucpu = std::make_unique<DCPU16N>();
				this->vc.SetCPU(std::move(dcpucpu));
				this->vc.On();
				this->vc.SetState(&state, sizeof(DCPU16NState));
			}
			break;
			case proto::Computer::CPU::CPU_NOT_SET:
			break;

		}
		std::string buf = comp.ram();
		trillek::Byte* ram = this->vc.Ram();
		for (std::size_t i = 0; i < buf.length(); ++i) {
			ram[i] = buf[i];
		}
		for (int i = 0; i < comp.devices_size(); ++i) {
			const proto::Computer::Device& device = comp.devices(i);
			switch (device.device_case()) {
				case proto::Computer::Device::kComputerScreen:
				{
					std::shared_ptr<ComputerScreen> screen = std::make_shared<ComputerScreen>();
					screen->In(device);
					this->vc.AddDevice(device.slot(), screen->device);
					this->devices[device.slot()] = screen;
				}
				break;
				case proto::Computer::Device::kComputerKeyboard:
				{
					std::shared_ptr<ComputerKeyboard> keybaord = std::make_shared<ComputerKeyboard>();
					keybaord->In(device);
					this->vc.AddDevice(device.slot(), keybaord->device);
					this->devices[device.slot()] = keybaord;
				}
				break;
				case proto::Computer::Device::DEVICE_NOT_SET:
				break;
			}
		}

		std::string fname{ comp.rom_file() };
		int size = LoadROM(FilePath::GetAssetPath(fname).toString(), this->rom);
		if (size < 0) {
			std::shared_ptr<spdlog::logger> _log = spdlog::get("console_log");
			_log->error("An error hapen when was reading the file {}", fname);
		}
		else {
			this->rom_size = size;
			this->rom_name = fname;
			this->vc.SetROM(this->rom, this->rom_size);
		}
	}

	void Computer::Out(proto::Component* target) {
		proto::Computer* comp = target->mutable_computer();
		for (auto dev : this->devices) {
			proto::Computer::Device* device = comp->add_devices();
			device->set_slot(dev.first);
			dev.second->Out(device);
		}
		comp->set_ram(this->vc.Ram(), this->vc.RamSize());
		proto::Computer::CPU* cpu = comp->mutable_cpu();
		proto::Computer::CPU::TR3200* tr3200 = cpu->mutable_tr3200();
		TR3200State state;
		this->vc.GetState(&state, sizeof(TR3200State));
		for (auto reg : state.r) {
			tr3200->add_registers(reg);
		}
		tr3200->set_pc(state.pc);
		tr3200->set_wait_cycles(state.wait_cycles);
		tr3200->set_int_msg(state.int_msg);
		tr3200->set_interrupt(state.interrupt);
		tr3200->set_step_mode(state.step_mode);
		tr3200->set_skiping(state.skiping);
		tr3200->set_sleeping(state.sleeping);
	}

	ComputerScreen::ComputerScreen() {
		this->device = std::make_shared<tda::TDADev>();
	}

	void ComputerScreen::In(const proto::Computer::Device& source) {
		const proto::ComputerScreen& comp = source.computerscreen();
		tda::TDAState state;
		// FIXME: presently GetState for TDA screen doesn't retrieve these buffers.
		// Therefore they are not save and we don't need to load them.
		/*std::string buf = comp.text_buffer();
		for (std::size_t i = 0, j = 0; i < buf.length(); ++i, ++j) {
			state.txt_buffer[j] = buf[i++] + (buf[i] << 8);
		}
		buf.clear();
		buf = comp.font_buffer();
		for (std::size_t i = 0, j = 0; i < buf.length(); ++i, ++j) {
			state.font_buffer[j] = buf[i++] + (buf[i] << 8);
		}*/
		state.buffer_ptr = comp.buffer_ptr();
		state.font_ptr = comp.font_ptr();
		state.vsync_msg = static_cast<trillek::Word>(comp.vsync_msg());
		state.a = static_cast<trillek::Word>(comp.a());
		state.b = static_cast<trillek::Word>(comp.b());
		state.d = static_cast<trillek::Word>(comp.d());
		state.e = static_cast<trillek::Word>(comp.e());
		std::size_t state_size = sizeof(tda::TDAState);
		this->device->SetState(&state, state_size);
	}

	void ComputerScreen::Out(proto::Computer::Device* target) {
		proto::ComputerScreen* comp = target->mutable_computerscreen();
		tda::TDAState state;
		std::size_t state_size = sizeof(tda::TDAState);
		this->device->GetState(&state, state_size);
		tda::TDAScreen screen;
		std::static_pointer_cast<tda::TDADev>(this->device)->DumpScreen(screen);
		// FIXME: presently GetState for TDA screen doesn't retrieve these buffers.
		// Therefore they are not save and we don't need to load them.
		//comp->set_text_buffer(&screen.txt_buffer, tda::TXT_BUFFER_SIZE);
		//comp->set_font_buffer(&screen.font_buffer, tda::FONT_BUFFER_SIZE);
		comp->set_buffer_ptr(state.buffer_ptr);
		comp->set_font_ptr(state.font_ptr);
		comp->set_vsync_msg(state.vsync_msg);
		comp->set_a(state.a);
		comp->set_b(state.b);
		comp->set_d(state.d);
		comp->set_e(state.e);
	}

	ComputerKeyboard::ComputerKeyboard() {
		this->device = std::make_shared<gkeyboard::GKeyboardDev>();
	}

	void ComputerKeyboard::In(const proto::Computer::Device&) {}

	void ComputerKeyboard::Out(proto::Computer::Device*) {}

	VComputerSystem::VComputerSystem() {
		_log = spdlog::get("console_log");
	};

	void VComputerSystem::SetDevice(const eid entity_id, const unsigned int slot, std::shared_ptr<DeviceBase> device) {
		if (this->computers.find(entity_id) != this->computers.end()) {
			this->computers[entity_id]->vc.AddDevice(slot, device->device);
			this->computers[entity_id]->devices[slot] = device;
		}
	}

	void VComputerSystem::RemoveDevice(const eid entity_id, const unsigned int slot) {
		if (this->computers.find(entity_id) != this->computers.end()) {
			auto dev = this->computers[entity_id]->vc.GetDevice(slot);
			if (this->computers[entity_id]->devices.find(slot) != this->computers[entity_id]->devices.end()) {
				if (this->computers[entity_id]->devices[slot]->device == dev) {
					this->computers[entity_id]->devices.erase(slot);
				}
			}
			this->computers[entity_id]->vc.RmDevice(slot);
		}
	}

	void VComputerSystem::Update(double _delta) {
		EventQueue<KeyboardEvent>::ProcessEventQueue();
		EventQueue<MouseBtnEvent>::ProcessEventQueue();
		EventQueue<EntityCreated>::ProcessEventQueue();
		EventQueue<EntityDestroyed>::ProcessEventQueue();
		for (auto computer_itr = ComputerComponentMap::Begin(); computer_itr != ComputerComponentMap::End(); ++computer_itr) {
			if (this->computers.find(computer_itr->first) == this->computers.end()) {
				this->computers[computer_itr->first] = computer_itr->second;
			}
		}
		ProcessCommandQueue();
		this->delta = _delta;
		tda::TDAScreen screen;
		//		static PixelBuffer local_pbuffer(320, 240, 8, ImageColorMode::COLOR_RGBA);
		for (const auto& comp : this->computers) {
			std::shared_ptr<ComputerScreen> comp_screen = std::static_pointer_cast<ComputerScreen>(comp.second->devices[5]);
			comp.second->vc.Update(this->delta);
			// #ifdef CLIENT_STANDALONE
			// 			std::static_pointer_cast<tda::TDADev>(comp_screen->device)->DumpScreen(screen);
			// 			tda::TDAtoRGBATexture(screen, (std::uint32_t*)local_pbuffer.LockWrite());
			// 			local_pbuffer.UnlockWrite();
			// 			if (comp_screen->texture) {
			// 				comp_screen->texture->Load(local_pbuffer);
			// 			}
			// 			else {
			// 				Entity screen_entity(comp.first);
			// 				if (screen_entity.Has<Renderable>()) {
			// 					Renderable* ren = screen_entity.Get<Renderable>();
			// 					if (ren->buffer) {
			// 						if (ren->buffer->GetVertexGroupCount() > 0) {
			// 							comp_screen->texture = ren->buffer->GetVertexGroup(0)->material->GetTexture(0);
			// 						}
			// 					}
			// 				}
			// 			}
			// #endif
		}
	}

	bool VComputerSystem::LoadROMFile(const eid entity_id, std::string fname) {
		if (this->computers.find(entity_id) != this->computers.end()) {
			auto computer = this->computers[entity_id];
			int size = LoadROM(fname, computer->rom);
			if (size < 0) {
				_log->error("An error hapen when was reading the file {}", fname);
				return false;
			}
			computer->rom_size = size;
			computer->rom_name = fname;
			computer->vc.SetROM(computer->rom, computer->rom_size);
			return true;
		}
		return false;
	}

	void VComputerSystem::TurnComptuerOn(const eid entity_id) {
		if (this->computers.find(entity_id) != this->computers.end()) {
			this->computers[entity_id]->vc.On();
		}
	}

	void VComputerSystem::TurnComptuerOff(const eid entity_id) {
		if (this->computers.find(entity_id) != this->computers.end()) {
			this->computers[entity_id]->vc.Off();
		}
	}

	void VComputerSystem::On(std::shared_ptr<KeyboardEvent> data) {
		std::shared_ptr<gkeyboard::GKeyboardDev> active_keybaord;
		for (auto keyboard_itr = KeyboardComponentMap::Begin();
			 keyboard_itr != KeyboardComponentMap::End(); ++keyboard_itr) {
			if (keyboard_itr->second->has_focus) {
				active_keybaord = std::static_pointer_cast<gkeyboard::GKeyboardDev>(keyboard_itr->second->device);
			}
		}
		if (active_keybaord) {
			switch (data->action) {
				case KeyboardEvent::KEY_DOWN:
				if (data->key == GLFW_KEY_ESCAPE) {
					if (KeyboardComponentMap::Has(this->active_entity)) {
						KeyboardComponentMap::Get(this->active_entity)->has_focus = false;
					}
				}
				else if (data->key == GLFW_KEY_BACKSPACE) {
					active_keybaord->SendKeyEvent(static_cast<trillek::Word>(data->scancode), gkeyboard::KEY_BACKSPACE,
												  gkeyboard::KEY_MODS::KEY_MOD_NONE);
				}
				else {
					active_keybaord->SendKeyEvent(static_cast<trillek::Word>(data->scancode), static_cast<unsigned char>(data->key),
												  gkeyboard::KEY_MODS::KEY_MOD_NONE);
				}
				default:
				break;
			}
		}
	}

	void VComputerSystem::On(std::shared_ptr<MouseBtnEvent> data) {
		if (data->action == MouseBtnEvent::DOWN && data->button == MouseBtnEvent::LEFT) {
			for (auto keyboard_itr = KeyboardComponentMap::Begin();
				 keyboard_itr != KeyboardComponentMap::End(); ++keyboard_itr) {
				if (keyboard_itr->first == this->active_entity) {
					keyboard_itr->second->has_focus = true;
				}
			}
		}
	}
	void VComputerSystem::On(std::shared_ptr<EntityCreated> data) {
		eid entity_id = data->entity.id();
		for (int i = 0; i < data->entity.components_size(); ++i) {
			const proto::Component& comp = data->entity.components(i);
			switch (comp.component_case()) {
				case proto::Component::kComputer:
				{
					Computer* computer = new Computer();
					computer->In(comp);
					ComputerComponentMap::Set(entity_id, computer);
				}
				break;
				case proto::Component::kCollisionBody:
				case proto::Component::kRenderable:
				case proto::Component::kPosition:
				case proto::Component::kOrientation:
				case proto::Component::kView:
				case proto::Component::kAnimation:
				case proto::Component::kScale:
				case proto::Component::kVelocity:
				case proto::Component::kAudioSource:
				case proto::Component::kPointLight:
				case proto::Component::kDirectionalLight:
				case proto::Component::kSpotLight:
				case proto::Component::kVoxelVolume:
				case proto::Component::kLuaScript:
				case proto::Component::COMPONENT_NOT_SET:
				break;
			}
		}
	}

	void VComputerSystem::On(std::shared_ptr<EntityDestroyed> data) {
		ComputerComponentMap::Remove(data->entity_id);
	}
}
