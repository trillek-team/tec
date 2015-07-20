#include "vcomputer-system.hpp"
#include "os.hpp"
#include "entity.hpp"

#include "vcomputer.hpp"
#include "auxiliar.hpp"
#include "tr3200/tr3200.hpp"

#include "graphics/texture-object.hpp"
#include "graphics/material.hpp"
#include "graphics/vertex-buffer-object.hpp"
#include "components/renderable.hpp"

namespace tec {
	using namespace trillek::computer;

	VComputerSystem::VComputerSystem() { };

	VComputerSystem::~VComputerSystem() { }

	void VComputerSystem::AddComputer(const eid entity_id) {
		std::unique_ptr<VComputer> vc(new VComputer());
		std::unique_ptr<TR3200> trcpu(new TR3200());
		vc->SetCPU(std::move(trcpu));
		this->computers[entity_id].vc = std::move(vc);
	}

	void VComputerSystem::SetDevice(const eid entity_id, const unsigned int slot, std::shared_ptr<Device> device) {
		if (this->computers.find(entity_id) != this->computers.end()) {
			this->computers[entity_id].vc->AddDevice(slot, device);
			this->computers[entity_id].devices.push_back(device);
		}
	}

	void VComputerSystem::RemoveComputer(const eid entity_id) {
		if (this->computers.find(entity_id) != this->computers.end()) {
			this->computers[entity_id].vc->Off();
			this->computers.erase(entity_id);
		}
	}

	void VComputerSystem::RemoveDevice(const eid entity_id, const unsigned int slot) {
		if (this->computers.find(entity_id) != this->computers.end()) {
			auto dev = this->computers[entity_id].vc->GetDevice(slot);
			for (auto itr = this->computers[entity_id].devices.begin(); itr != this->computers[entity_id].devices.end(); ++itr) {
				if ((*itr) == dev) {
					this->computers[entity_id].devices.erase(itr);
					break;
				}
			}
			this->computers[entity_id].vc->RmDevice(slot);
		}
	}

	void VComputerSystem::Update(double delta) {
		ProcessCommandQueue();
		EventQueue<KeyboardEvent>::ProcessEventQueue();
		this->delta = delta;
		for (const auto& comp : this->computers) {
			comp.second.vc->Update(delta);
		}
		static PixelBuffer local_pbuffer(320, 240, 8, ImageColorMode::COLOR_RGBA);
		tda::TDAScreen screen;
		for (auto screen_itr = ScreenComponentMap::Begin();
			screen_itr != ScreenComponentMap::End(); ++screen_itr) {
			screen_itr->second->device->DumpScreen(screen);
			tda::TDAtoRGBATexture(screen, (std::uint32_t*)local_pbuffer.LockWrite());
			local_pbuffer.UnlockWrite();
			if (screen_itr->second->texture) {
				screen_itr->second->texture->Load(local_pbuffer);
			}
			else {
				Entity screen_entity(screen_itr->first);
				if (screen_entity.Has<Renderable>()) {
					std::shared_ptr<Renderable> ren = screen_entity.Get<Renderable>().lock();
					if (ren->buffer) {
						if (ren->buffer->GetVertexGroupCount() > 0) {
							screen_itr->second->texture = ren->buffer->GetVertexGroup(0)->material->GetTexutre(0);
						}
					}
				}
			}
		}
	}


	bool VComputerSystem::LoadROMFile(const eid entity_id, std::string fname) {
		if (this->computers.find(entity_id) != this->computers.end()) {
			auto& vc = this->computers[entity_id].vc;
			int size = LoadROM(fname, this->computers[entity_id].rom);
			if (size < 0) {
				std::fprintf(stderr, "An error hapen when was reading the file %s\n", fname.c_str());
				return false;
			}
			this->computers[entity_id].rom_size = size;
			vc->SetROM(this->computers[entity_id].rom, this->computers[entity_id].rom_size);
			return true;
		}
		return false;
	}

	void VComputerSystem::TurnComptuerOn(const eid entity_id) {
		if (this->computers.find(entity_id) != this->computers.end()) {
			this->computers[entity_id].vc->On();
		}
	}

	void VComputerSystem::TurnComptuerOff(const eid entity_id) {
		if (this->computers.find(entity_id) != this->computers.end()) {
			this->computers[entity_id].vc->Off();
		}
	}

	void VComputerSystem::On(std::shared_ptr<KeyboardEvent> data) {
		std::shared_ptr<gkeyboard::GKeyboardDev> active_keybaord;
		for (auto keyboard_itr = KeyboardComponentMap::Begin();
			keyboard_itr != KeyboardComponentMap::End(); ++keyboard_itr) {
			if (keyboard_itr->second->has_focus) {
				active_keybaord = keyboard_itr->second->device;
			}
		}
		if (active_keybaord) {
			switch (data->action) {
				case KeyboardEvent::KEY_DOWN:
					active_keybaord->SendKeyEvent(data->scancode, data->key,
						gkeyboard::KEY_MODS::KEY_MOD_NONE);
				default:
					break;
			}
		}
	}
}
