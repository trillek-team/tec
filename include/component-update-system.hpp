#pragma once

#include <mutex>
#include <list>
#include <deque>
#include <memory>
#include <cstdint>
#include <functional>
#include "multiton.hpp"
#include "types.hpp"
#include "events.hpp"
#include "reflection.hpp"

namespace tec {
	extern ReflectionEntityList entity_list;
	extern std::map<eid, std::shared_ptr<EnttityComponentUpdatedEvent>> entities_updated;

	template <typename T>
	struct ComponentUpdateList {
		ComponentUpdateList() { }
		ComponentUpdateList(ComponentUpdateList&& other) : frame(other.frame) {
			updates = std::move(other.updates);
			removals = std::move(other.removals);
		}
		frame_id_t frame = 0; // The frame id the changes are in.
		std::map<eid, std::shared_ptr<T>> updates; // Components to be updated
		std::list<eid> removals; // Components to be removed
	};

	// Used to iterate over all ComponentUpdateSystem::UpdateTo methods.
	class ComponentUpdateSystemList {
	public:
		ComponentUpdateSystemList() { }

		// Update all ComponentUpdateSystem to frame_id by calling their update functions
		// that was registered in ComponentUpdateSystem::Initialize().
		static void UpdateAll(frame_id_t frame_id) {
			for (auto func : update_funcs) {
				func(frame_id);
			}
		}

		// Register an update function
		static void RegisterUpdateFunction(std::function<void(frame_id_t)>&& update_func) {
			update_funcs.push_back(update_func);
		}
	private:
		static std::list<std::function<void(frame_id_t)>> update_funcs;
	};

	// Schedules updates and removals of component's and applies them to
	// the central component store when UpdateTo is called.
	template <typename T>
	class ComponentUpdateSystem {
	public:
		ComponentUpdateSystem() { }
		~ComponentUpdateSystem() { }

		static void Initialize() {
			auto update_func = [ ] (frame_id_t frame_id) {
				ComponentUpdateSystem<T>::UpdateTo(frame_id);
			};
			ComponentUpdateSystemList::RegisterUpdateFunction(update_func);
		}

		// Applies all future_update_lists up to and including frame_id
		static void UpdateTo(frame_id_t frame_id) {
			{
				std::lock_guard<std::mutex> list_lock(list_mutex);
				base_frame_id = frame_id;
			}
			while (!future_update_lists.empty()) {
				std::unique_ptr<ComponentUpdateList<T>> front;
				{
					std::lock_guard<std::mutex> list_lock(list_mutex);
					front = std::move(future_update_lists.front());
					future_update_lists.pop_front();
				}
				if (front->frame <= frame_id) {
					const std::map<eid, std::shared_ptr<T>>& updates = front->updates;
					const char* component_type_name = GetTypeName<T>();
					entities_updated.clear();
					for (auto pair : updates) {
						Multiton<eid, std::shared_ptr<T>>::Set(pair.first, pair.second);
						if (entities_updated[pair.first] == nullptr) {
							entities_updated[pair.first] = std::make_shared<EnttityComponentUpdatedEvent>();
							entities_updated[pair.first]->entity.set_id(pair.first);
						}
						proto::Component* comp = entities_updated[pair.first]->entity.add_components();
						pair.second->Out(comp);
						entity_list.entities[pair.first].components[component_type_name] = std::move(T::Reflection(pair.second.get()));
					}

					const std::list<eid>& removals = front->removals;
					for (auto entity_id : removals) {
						Multiton<eid, std::shared_ptr<T>>::Remove(entity_id);
						if (entity_list.entities[entity_id].components.find(component_type_name) != entity_list.entities[entity_id].components.end()) {
							entity_list.entities[entity_id].components.erase(component_type_name);
						}
					}
					front->frame = -1;
					front->updates.clear();
					front->removals.clear();
					{
						std::lock_guard<std::mutex> list_lock(list_mutex);
						empty_update_lists.push_back(std::move(front));
					}
				}
				else {
					break;
				}
			}
		}

		// Schedule a component update for frame_id or the most future if frame_id is 0.
		static void SubmitUpdate(eid entity_id, std::shared_ptr<T> value, frame_id_t frame_id = 0) {
			std::lock_guard<std::mutex> list_lock(list_mutex);
			ComponentUpdateList<T>* list = nullptr;
			auto it = future_update_lists.begin();
			if (frame_id > base_frame_id) {
				for (it; it != future_update_lists.end(); it++) {
					if ((*it)->frame == frame_id) {
						list = it->get();
						break;
					}
					else if ((*it)->frame > frame_id) {
						break;
					}
				}
			}
			if (!list) {
				if (future_update_lists.empty()) {
					if (empty_update_lists.size() > 0) {
						it = future_update_lists.emplace(it, std::move(empty_update_lists.front()));
						empty_update_lists.pop_front();
					}
					else {
						it = future_update_lists.emplace(it, std::make_unique<ComponentUpdateList<T>>());
					}
				}
			}
			list = it->get();
			if (frame_id == 0) {
				list->frame = base_frame_id + 1;
			}
			else {
				list->frame = frame_id;
			}
			list->updates[entity_id] = value;
		}

		// Schedule a component removal for frame_id or the most future if frame_id is 0.
		static void SubmitRemoval(eid entity_id, frame_id_t frame_id = 0) {
			std::lock_guard<std::mutex> list_lock(list_mutex);
			ComponentUpdateList<T>* list = nullptr;
			auto it = future_update_lists.begin();
			if (frame_id > base_frame_id) {
				for (it; it != future_update_lists.end(); it++) {
					if ((*it)->frame == frame_id) {
						list = it->get();
						break;
					}
					else if ((*it)->frame > frame_id) {
						break;
					}
				}
			}
			if (!list) {
				if (future_update_lists.empty()) {
					if (empty_update_lists.size() > 0) {
						it = future_update_lists.emplace(it, std::move(empty_update_lists.front()));
						empty_update_lists.pop_front();
					}
					else {
						it = future_update_lists.emplace(it, std::make_unique<ComponentUpdateList<T>>());
					}
				}
				list = it->get();
				if (frame_id == 0) {
					list->frame = base_frame_id + 1;
				}
				else {
					list->frame = frame_id;
				}
			}
			list->removals.push_back(entity_id);
		}
	protected:
		static std::deque<std::unique_ptr<ComponentUpdateList<T>>> future_update_lists;
		static std::list<std::unique_ptr<ComponentUpdateList<T>>> empty_update_lists;
		static std::mutex list_mutex;

		// The last confirmed frame_id
		static frame_id_t base_frame_id;
	};

	template <typename T>
	std::deque<std::unique_ptr<ComponentUpdateList<T>>> ComponentUpdateSystem<T>::future_update_lists;
	template <typename T>
	std::list<std::unique_ptr<ComponentUpdateList<T>>> ComponentUpdateSystem<T>::empty_update_lists;

	template <typename T>
	frame_id_t ComponentUpdateSystem<T>::base_frame_id = -1;

	template <typename T>
	std::mutex ComponentUpdateSystem<T>::list_mutex;
}
