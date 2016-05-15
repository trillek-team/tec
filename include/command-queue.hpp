// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <queue>
#include <memory>
#include <functional>
#include <atomic>

namespace tec {
	template <class T>
	struct Command {
		Command(std::function<void(T*)>&& command) : command(std::move(command)) { }
		Command(Command&& c) : command(std::move(c.command)) { }
		std::function<void(T*)> command;
	};

	// Thread friendly double queue for incoming commands. Call ProcessCommandQueue() to
	// iterate over all queued commands when it is safe to modify state.
	template <class T>
	class CommandQueue {
	public:
		CommandQueue() : local_queue(new std::queue<Command<T>>()) {
			if (!global_command_queue.load()) {
				global_command_queue.store(new std::queue<Command<T>>());
			}
		}
		~CommandQueue() { }

		void ProcessCommandQueue() {
			this->local_queue = global_command_queue.exchange(this->local_queue);

			while (!this->local_queue->empty()) {
				Command<T> command = std::move(this->local_queue->front());
				this->local_queue->pop();

				command.command(static_cast<T*>(this));
			}
		}

		static void QueueCommand(Command<T>&& command) {
			(*global_command_queue).push(std::move(command));
		}

		static void QueueCommand(std::function<void(T*)>&& command) {
			(*global_command_queue).push(std::move(command));
		}
	protected:
		static std::atomic<std::queue<Command<T>>*> global_command_queue;
		std::queue<Command<T>>* local_queue;
	};

	template <class T>
	std::atomic<std::queue<Command<T>>*> CommandQueue<T>::global_command_queue;
}
