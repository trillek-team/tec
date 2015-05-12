#pragma once

#include <list>
#include <map>
#include <memory>
#include <mutex>

#include "types.hpp"
#include "event-queue.hpp"

namespace vv {
	template <typename T>
	class EventSystem final {
	private:
		EventSystem() { }
		EventSystem(const EventSystem& right) {
			instance = right.instance;
		}
		EventSystem& operator=(const EventSystem& right) {
			if (this != &right) {
				instance = right.instance;
			}

			return *this;
		}
		static std::once_flag only_one;
		static std::shared_ptr<EventSystem<T>> instance;
	public:
		static std::shared_ptr<EventSystem<T>> Get() {
			std::call_once(EventSystem::only_one,
				[ ] () {
				EventSystem<T>::instance.reset(new EventSystem<T>());
			}
			);

			return EventSystem<T>::instance;
		}
		~EventSystem() { }

		/**
		 * \brief Subscribes to be notified of events
		 *
		 * \param const unsigned int entity_id ID of the entity to subscribe to.
		 * \param const EventQueue<T>* subscriber The subscriber to add.
		 * \return void
		 */
		void Subscribe(const eid entity_id, EventQueue<T>* subscriber) {
			auto subs = this->subscribers.find(entity_id);
			if (subs == this->subscribers.end()) {
				// no subscriptions for entity, add the entity and subscriber
				this->subscribers[entity_id].push_back(subscriber);
				return;
			}
			else {
				// check if subscriber already exists
				for (auto sub : subs->second) {
					if (sub == subscriber) {
						return; // already subscribed
					}
				}
				subs->second.push_back(subscriber);
			}
		}

		/**
		 * \brief Subscribes to be notified of events for all entity IDs.
		 *
		 * \param const Receiver<T>* subscriber The subscriber to add.
		 * \return void
		 */
		void Subscribe(EventQueue<T>* subscriber) {
			this->subscribers[0].push_back(subscriber);
		}

		/**
		 * \brief Unsubscribes to notification of events.
		 *
		 * \param const unsigned int entity_id ID of the entity to unsubscribe from.
		 * \param const Receiver<T>* subscriber The subscriber to remove.
		 * \return void
		 */
		void Unsubscribe(const eid entity_id, EventQueue<T>* subscriber) {
			if (this->subscribers.find(entity_id) != this->subscribers.end()) {
				this->subscribers[entity_id].remove(subscriber);
			}
		}

		/**
		 * \brief Unsubscribes to notification of events.
		 *
		 * \param const Receiver<T>* subscriber The subscriber to remove.
		 * \return void
		 */
		void Unsubscribe(EventQueue<T>* subscriber) {
			if (this->subscribers.find(0) != this->subscribers.end()) {
				this->subscribers[0].remove(subscriber);
			}
		}

		/**
		 * \brief Emits an event to subscribers for a given entity_id and to all
		 * subscribers listening for events for any entity_id.
		 *
		 * \param const unsigned int entity_id ID of the entity the event happen to.
		 * \param const T* data The event data.
		 * \return void
		 */
		void Emit(const eid entity_id, std::shared_ptr<T> data) {
			if (this->subscribers.find(entity_id) != this->subscribers.end()) {
				auto subscriber_list = this->subscribers.at(entity_id);
				for (EventQueue<T>* subscriber : subscriber_list) {
					subscriber->On(entity_id, data);
				}
			}

			if (this->subscribers.find(0) != this->subscribers.end()) {
				auto subscriber_list = this->subscribers.at(0);
				for (EventQueue<T>* subscriber : subscriber_list) {
					subscriber->On(entity_id, data);
				}
			}
		}

		/**
		 * \brief Emits an event to all subscribers listening for events for any entity_id.
		 *
		 * \param const T* data The changed data.
		 * \return void
		 */
		void Emit(std::shared_ptr<T> data) {
			if (this->subscribers.find(0) != this->subscribers.end()) {
				auto subscriber_list = this->subscribers.at(0);
				for (EventQueue<T>* subscriber : subscriber_list) {
					Event<T> e(0, data);
					subscriber->QueueEvent(std::move(e));
				}
			}
		}

	private:
		std::map<eid, std::list<EventQueue<T>*>> subscribers;
	};

	template<typename T>
	std::once_flag EventSystem<T>::only_one;

	template<typename T>
	std::shared_ptr<EventSystem<T>> EventSystem<T>::instance = nullptr;
}
