#pragma once
/**
 * Ring buffer implementation
 */

#include <array>

namespace tec {
	template < class T, std::size_t N >
	class RingBuffer {
	public:

		/**
		 * Return element pos of the buffer. Not bounds checks
		 */
		const T& operator[](std::size_t pos) const {
			auto p = (read + pos) % N;
			return buffer[p];
		}

		/** 
		 * Returns the last element on the container (ie, the most older)
		 */
		T& back() {
			return buffer[read];
		}
		/** 
		 * Returns the last element on the container (ie, the most older)
		 */
		const T& back() const {
			return buffer[read];
		}

		/**
		 * Remove the last element on the ring buffer 
		 */
		void pop_back() {
			if (! this->empty()) {
				read = (read + 1) % N;
				elements--;
			}
		}

		/**
		 * Prepends the given element value to the begining of the buffer
		 */
		void push_front(const T& value) {
			if (! this->full()) {
				buffer[write] = value;
				write = (write + 1) % N;
				elements++;
			}
		}

		/**
		 * Returns tru if there is not any element stored
		 */
		bool empty() const {
			return this->elements == 0;
		}
		
		/**
		 * Returns tru if there is not any element stored
		 */
		bool full() const {
			return this->elements == N;
		}
		
		/**
		 * Returns the number of items stored
		 */
		std::size_t size() const {
			return this->elements;
		}
		
		/**
		 * Returns the max number of elements that can be stored (ie N)
		 */
		std::size_t max_size() const {
			return N;
		}

		/**
		 * Removes all elements of the buffer
		 */
		void clear() {
			write = 0;
			read = 0;
			elements = 0;
		}

	private:
		std::array<T, N> buffer;

		std::size_t write = 0;     /// Were to write
		std::size_t read = 0;      /// Were to read
		std::size_t elements = 0;  /// Stored number of elements
	};
}
