#pragma once

#include "vcomputer.hpp"
#include "types.hpp"
#include <memory>
#include <list>
#include <cstdint>
#include "resources/pixel-buffer.hpp"
#include "devices/tda.hpp"
#include "devices/gkeyb.hpp"

#include "event-system.hpp"
#include "reflection.hpp"
#include "command-queue.hpp"
#include "multiton.hpp"

namespace tec {
	using namespace trillek::computer;

	class VComputerSystem;
	typedef Command<VComputerSystem> VComputerCommand;

	// Struct to hold various CPU specific items together.
	struct Computer final {
		Computer() : rom(new std::uint8_t[32 * 1024]), rom_size(0) { }
		~Computer() {
			if (this->rom) {
				this->vc->Off();
				delete this->rom;
			}
		}
		std::uint8_t *rom;
		size_t rom_size;
		std::unique_ptr<VComputer> vc;
		std::list<std::shared_ptr<Device>> devices;
		
		static ReflectionComponent Reflection(Computer* val) {
			ReflectionComponent refcomp;
			return std::move(refcomp);
		}
	};

	class TextureObject;
	struct ComputerScreen {
		ComputerScreen() {
			this->device = std::make_shared<tda::TDADev>();
		}
		std::shared_ptr<TextureObject> texture;
		std::shared_ptr<tda::TDADev> device;

		static ReflectionComponent Reflection(ComputerScreen* val) {
			ReflectionComponent refcomp;
			return std::move(refcomp);
		}
	};

	struct ComputerKeyboard {
		ComputerKeyboard() : has_focus(false) {
			this->device = std::make_shared<gkeyboard::GKeyboardDev>();
		}
		bool has_focus;
		std::shared_ptr<gkeyboard::GKeyboardDev> device;

		static ReflectionComponent Reflection(ComputerKeyboard* val) {
			ReflectionComponent refcomp;
			return std::move(refcomp);
		}
	};

	struct KeyboardEvent;

	class VComputerSystem final : public CommandQueue < VComputerSystem >,
		public EventQueue < KeyboardEvent > {
	public:
		VComputerSystem();
		~VComputerSystem();

		/** \brief Add a computer/CPU to the system.
		 *
		 * \param const eid entityID The entity ID the computer/CPU belongs to.
		 * \param CPU_TYPE The type of CPU to "install" in the computer.
		 */
		void AddComputer(const eid entity_id);

		/** \brief Removes a computer/CPU from the system.
		*
		* \param const eid entityID The entity ID the computer/CPU belongs to.
		*/
		void RemoveComputer(const eid entity_id);

		/** \brief Sets the specified device for the entity ID to device.
		 *
		 * \param const eid entityID The entity ID the computer/CPU belongs to.
		 * \param const unsigned int The slot to assign device to.
		 * \param std::shared_ptr<IDevice> device The device to install.
		 */
		void SetDevice(const eid entity_id, const unsigned int slot, std::shared_ptr<Device> device);

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
		 * \return bool Wether or not the file loaded successfully (also returns false if no computer exists for the given entity_id).
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

		void On(std::shared_ptr<KeyboardEvent> data);
	private:
		typedef Multiton<eid, std::shared_ptr<ComputerScreen>> ScreenComponentMap;
		typedef Multiton<eid, std::shared_ptr<ComputerKeyboard>> KeyboardComponentMap;

		double delta; // The time since the last Update was called.

		std::map<eid, Computer> computers;
	};

}
