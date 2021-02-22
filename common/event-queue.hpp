#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <queue>

#include "tec-types.hpp"

namespace tec {

// Container to hold event data. This is stored in the queue rather than raw event data.
template <class T> struct Event {
	Event(eid entity_id, std::shared_ptr<T> data) : entity_id(entity_id), data(data) {}
	Event(Event&& other) noexcept : entity_id(other.entity_id), data(other.data) {}
	eid entity_id;
	std::shared_ptr<T> data;
};
// Helper class containing a std::queue. The std::queue is NOT thread safe
// so we can end up with an "emplace" in-flight while the queue is being read
// this can lead to a race condition where the item is pop()'ed before emplace finishes
// that is all kinds of bad, so this wraps it with a simple spinlock if a write was happening
template <class T> struct Queue {
	Queue() {}
	void emplace(T&& e) {
		std::scoped_lock<std::mutex> lock(queue_write);
		queue.emplace(std::move(e));
	}
	std::queue<T> queue;
	std::mutex queue_write;
};

template <typename T> class EventSystem;

// Thread friendly double queue for incoming events. Call EventQueue<T>::ProcessEventQueue()
// to iterate over all queued events when it is safe to modify state. You must
// qualify the call with the base class and template type to avoid ambiguity.
template <class T> class EventQueue {
public:
	EventQueue() : read_event_queue(new Queue<Event<T>>()), write_event_queue(new Queue<Event<T>>()) {
		EventSystem<T>::Get()->Subscribe(this);
	}
	// Causes subscribing to events for only a specific entity_id.
	EventQueue(eid entity_id) :
			read_event_queue(new std::queue<Event<T>>()), write_event_queue(new std::queue<Event<T>>()) {
		EventSystem<T>::Get()->Subscribe(entity_id, this);
	}
	virtual ~EventQueue() {}

	void ProcessEventQueue() {
		this->read_event_queue = write_event_queue.exchange(this->read_event_queue);
		std::scoped_lock<std::mutex> lock(this->read_event_queue->queue_write);
		while (!this->read_event_queue->queue.empty()) {
			Event<T> e = std::move(this->read_event_queue->queue.front());
			this->read_event_queue->queue.pop();

			static_cast<EventQueue<T>*>(this)->On(e.data);
		}
	}

	void QueueEvent(Event<T>&& e) { (*write_event_queue).emplace(std::move(e)); }

	virtual void On(const eid, std::shared_ptr<T>) {}
	virtual void On(std::shared_ptr<T>) {}

protected:
	Queue<Event<T>>* read_event_queue;
	std::atomic<Queue<Event<T>>*> write_event_queue;
};

} // end namespace tec
