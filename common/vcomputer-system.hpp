// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <memory>
#include <list>
#include <cstdint>

#include <vcomputer.hpp>
#include <spdlog/spdlog.h>

#include "types.hpp"
#include "event-system.hpp"
#include "command-queue.hpp"
#include "multiton.hpp"

namespace tec {
	using namespace trillek::computer;

	class VComputerSystem;
	typedef Command<VComputerSystem> VComputerCommand;

	struct DeviceBase {
		std::shared_ptr<Device> device;
		virtual void In(const proto::Computer::Device& source) = 0;

		virtual void Out(proto::Computer::Device* target) = 0;
	};

	// Struct to hold various CPU specific items together.
	struct Computer final {
		Computer();
		~Computer() {
			if (this->rom) {
				this->vc.Off();
				delete[] this->rom;
			}
		}
		std::uint8_t* rom;
		std::string rom_name;
		std::size_t rom_size;
		VComputer vc;
		std::map<int, std::shared_ptr<DeviceBase>> devices;

		void In(const proto::Component& source);

		void Out(proto::Component* target);
	};

	class TextureObject;
	struct ComputerScreen : DeviceBase {
		ComputerScreen();
		virtual ~ComputerScreen() = default;
		std::shared_ptr<TextureObject> texture;

		void In(const proto::Computer::Device& source);

		void Out(proto::Computer::Device* target);
	};

	struct ComputerKeyboard : DeviceBase {
		ComputerKeyboard();
		virtual ~ComputerKeyboard() = default;

		bool has_focus{ false };

		void In(const proto::Computer::Device& source);

		void Out(proto::Computer::Device* target);
	};

	struct KeyboardEvent;
	struct MouseBtnEvent;
	struct EntityCreated;
	struct EntityDestroyed;

	class VComputerSystem final :
		public CommandQueue<VComputerSystem>,
		public EventQueue<KeyboardEvent>,
		public EventQueue<MouseBtnEvent>,
		public EventQueue < EntityCreated >,
		public EventQueue < EntityDestroyed > {
	public:
		VComputerSystem();

		/** \brief Sets the specified device for the entity ID to device.
		 *
		 * \param const eid entityID The entity ID the computer/CPU belongs to.
		 * \param const unsigned int The slot to assign device to.
		 * \param std::shared_ptr<IDevice> device The device to install.
		 */
		void SetDevice(const eid entity_id, const unsigned int slot, std::shared_ptr<DeviceBase> device);

		/** \brief Remove a device from the specified slot.
		 *
		 * \param const eid entityID The entity ID the computer/CPU belongs to.
		 * \param const unsigned int The slot to remove the device from.
		 */
		void RemoveDevice(const eid entity_id, const unsigned int slot);

		/** \brief Load a ROM file from disk for the specified computer.
		 *
		 * \param const eid entityID The entity ID the computer belongs to.
		 * \param const std::string fname The name of the ROM file to load.
		 * \return bool Whether or not the file loaded successfully (also returns false if no computer exists for the given entity_id).
		 */
		bool LoadROMFile(const eid entity_id, std::string fname);

		/** \brief Turns specified the computer on.
		 *
		 * \param const eid entityID The entity ID the computer belongs to.
		 */
		void TurnComptuerOn(const eid entity_id);

		/** \brief Turns specified the computer off.
		 *
		 * \param const eid entityID The entity ID the computer belongs to.
		 */
		void TurnComptuerOff(const eid entity_id);

		/** \brief Handle incoming events to update data
		 *
		 * This function is called once every frame. It is the only
		 * function that can write data. This function is in the critical
		 * path, job done here must be simple.
		 *
		 * If event handling need some batch processing, a task list must be
		 * prepared and stored temporarily to be retrieved by RunBatch().
		 */
		void Update(double delta);

		using EventQueue<KeyboardEvent>::On;
		using EventQueue<MouseBtnEvent>::On;
		using EventQueue<EntityCreated>::On;
		using EventQueue<EntityDestroyed>::On;
		void On(std::shared_ptr<KeyboardEvent> data);
		void On(std::shared_ptr<MouseBtnEvent> data);
		void On(std::shared_ptr<EntityCreated> data);
		void On(std::shared_ptr<EntityDestroyed> data);
	private:
		std::shared_ptr<spdlog::logger> _log;
		typedef Multiton<eid, Computer*> ComputerComponentMap;
		typedef Multiton<eid, ComputerKeyboard*> KeyboardComponentMap;

		double delta{ 0 }; // The time since the last Update was called.

		std::map<eid, Computer*> computers;
	};

}
