#pragma once
#include <memory>

#include "physics-system.hpp"
#include "types.hpp"
#include "proto/components.pb.h"
#include "vcomputer-system.hpp"

namespace tec {
	class Simulation final {
	public:
		Simulation() { }
		~Simulation() { }

		void Simulate(const double delta_time);

		PhysicsSystem& GetPhysicsSystem() {
			return this->phys_sys;
		}
		
		VComputerSystem& GetVComputerSystem() {
			return this->vcomp_sys;
		}

		void CreateDummyData();

		std::map<eid, std::map<tid, proto::Component>> GetResults() {
			return std::move(entities_updated);
		}
	private:
		PhysicsSystem phys_sys;
		VComputerSystem vcomp_sys;
		std::map<eid, std::map<tid, proto::Component>> entities_updated;
	};
}
