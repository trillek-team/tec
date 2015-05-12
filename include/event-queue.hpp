#pragma once

#include <queue>
#include <memory>
#include <atomic>
#include "types.hpp"

namespace vv {
	// Container to hold event data. This is stored in the queue rather than raw event data.
	template <class T>
	struct Event {
		Event(eid entity_id, std::shared_ptr<T> data) : entity_id(entity_id), data(data) { }
		Event(Event&& other) : entity_id(other.entity_id), data(other.data) { }
		eid entity_id;
		std::shared_ptr<T> data;
	};

	template <typename T>
	class EventSystem;

	// Thread friendly double queue for incoming events. Call EventQueue<T>::ProcessEventQueue()
	// to iterate over all queued events when it is safe to modify state. You must
	// qualify the call with the base class and template type to avoid ambiguity.
	template <class T>
	class EventQueue {
	public:
		EventQueue() : read_event_queue(new std::queue<Event<T>>()),
			write_event_queue(new std::queue<Event<T>>()) {
			EventSystem<T>::Get()->Subscribe(this);
		}
		// Causes subscribing to events for only a specific entity_id.
		EventQueue(eid entity_id) : read_event_queue(new std::queue<Event<T>>()),
			write_event_queue(new std::queue<Event<T>>()) {
			EventSystem<T>::Get()->Subscribe(entity_id, this);
		}
		~EventQueue() { }

		void ProcessEventQueue() {
			this->read_event_queue = write_event_queue.exchange(this->read_event_queue);

			while (!this->read_event_queue->empty()) {
				Event<T> e = std::move(this->read_event_queue->front());
				this->read_event_queue->pop();

				static_cast<EventQueue<T>*>(this)->On(e.data);
			}
		}

		void QueueEvent(Event<T>&& e) {
			(*write_event_queue).push(std::move(e));
		}

		virtual void On(const eid entity_id, std::shared_ptr<T> data) { }
		virtual void On(std::shared_ptr<T> data) { }
	protected:
		std::atomic<std::queue<Event<T>>*> write_event_queue;
		std::queue<Event<T>>* read_event_queue;
	};
}
