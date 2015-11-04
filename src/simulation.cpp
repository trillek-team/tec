#include "simulation.hpp"

#include <thread>
#include <future>
#include <set>

#include "components/transforms.hpp"

#include "components/collisionbody.hpp"

namespace tec {
	void Simulation::CreateDummyData() {
		std::shared_ptr<CollisionBody> colbody = std::make_shared<CollisionBody>();
		colbody->new_collision_shape = COLLISION_SHAPE::BOX;
		colbody->disable_deactivation = true;
		colbody->mass = 1.0;
		Multiton<eid, std::shared_ptr<CollisionBody>>::Set(500, colbody);
		std::shared_ptr<Position> pos = std::make_shared<Position>(glm::vec3(1.0, 1.0, 1.0));
		Multiton<eid, std::shared_ptr<Position>>::Set(500, pos);
		std::shared_ptr<Velocity> vel = std::make_shared<Velocity>(glm::vec4(1.0, 0.0, 0.0,0.0), glm::vec4(0.0));
		Multiton<eid, std::shared_ptr<Velocity>>::Set(500, vel);
	}

	void Simulation::Simulate(const double delta_time) {
		std::future<std::set<eid>> phys_future = std::async(std::launch::async, [&] () -> std::set < eid > {
			return std::move(phys_sys.Update(delta_time));
		});
		/*auto vcomp_future = std::async(std::launch::async, [&] () {
			vcomp_sys.Update(delta_time);
		});*/
		entities_updated.clear();
		std::set<eid> phys_results = phys_future.get();
		for (eid entity_id : phys_results) {
			this->phys_sys.GetPosition(entity_id)->Out(&entities_updated[entity_id][GetTypeID<Position>()]);
			this->phys_sys.GetOrientation(entity_id)->Out(&entities_updated[entity_id][GetTypeID<Orientation>()]);
		}
		//vcomp_future.get();
	}
}
