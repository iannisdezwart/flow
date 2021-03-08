#ifndef FLOW_QUEUE_HEADER
#define FLOW_QUEUE_HEADER

#include "dynamic-array.hpp"

namespace flow_queue_tools {
	template <typename type>
	class QueueNode {
		public:
			QueueNode *next;
			type value;

			QueueNode(type value, QueueNode *next = NULL) : value(value), next(next) {}
	};
};

namespace flow {
	using namespace flow_queue_tools;

	enum class QueueErrors {
		POP_EMPTY_QUEUE,
		INDEX_OUT_OF_BOUNDS
	};

	template <typename type>
	class Queue {
		private:
			size_t current_element_count = 0;
			QueueNode<type> *first = NULL;
			QueueNode<type> *last = NULL;

		public:
			/**
			 *  @brief  Creates a Queue
			 */
			Queue() {}

			/**
			 *  @brief  Deletes all elements on this Queue.
			 *  @note  Runtime: O(n)
			 *  @note  Memory O(1)
			 */
			~Queue()
			{
				QueueNode<type> *node = first;

				for (size_t i = 0; i < current_element_count; i++) {
					QueueNode<type> *next_node = node->next;
					delete node;
					node = next_node;
				}
			}

			/**
			 *  @brief  Returns the number of elements on the Queue.
			 */
			size_t size() const
			{
				return current_element_count;
			}

			/**
			 *  @brief  Returns a reference to the value of the first element on
			 *  the Queue.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			const type& front() const
			{
				return first->value;
			}

			/**
			 *  @brief  Returns a reference to the value of the last element on
			 *  the Queue.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			const type& back() const
			{
				return last->value;
			}

			/**
			 *  @brief  Returns a read/write reference to the value at the i-th
			 *  element in the Queue.
			 *  @param  offset  i
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			type& operator[](size_t offset)
			{
				if (offset >= current_element_count) throw QueueErrors::INDEX_OUT_OF_BOUNDS;

				QueueNode<type> *node = first;
				for (size_t i = 0; i < offset; i++) node = node->next;

				return node->value;
			}

			/**
			 *  @brief  Returns a read-only reference to the value at the i-th
			 *  element in the Queue.
			 *  @param  offset  i
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			const type& operator[](size_t offset) const
			{
				if (offset >= current_element_count) throw QueueErrors::INDEX_OUT_OF_BOUNDS;

				QueueNode<type> *node = first;
				for (size_t i = 0; i < offset; i++) node = node->next;

				return node->value;
			}

			/**
			 *  @brief  Returns the offset to the first occurence of a given value.
			 *  Returns -1 if the value was not found.
			 *  @param  value  The value to find.
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			ssize_t first_index_of(const type& value) const
			{
				QueueNode<type> *node = first;

				for (size_t i = 0; i < current_element_count; i++) {
					if (node->value == value) return i;
					node = node->next;
				}

				return -1;
			}

			/**
			 *  @brief  Returns the indices all occurrences of a given value.
			 *  @param  value  The value to find the indices of.
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(n), n = found indices
			 */
			DynamicArray<size_t> indices_of(const type& value) const
			{
				DynamicArray<size_t> indices;
				QueueNode<type> *node = first;

				for (size_t i = 0; i < current_element_count; i++) {
					if (node->value == value) indices.append(i);
					node = node->next;
				}

				return indices;
			}

			/**
			 *  @brief  Swaps the values at two indices of the Queue.
			 *  @param  index_1  The first index.
			 *  @param  index_2  The second index.
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			void swap_indices(size_t index_1, size_t index_2)
			{
				if (index_1 == index_2) return;
				if (index_1 > current_element_count) throw QueueErrors::INDEX_OUT_OF_BOUNDS;
				if (index_2 > current_element_count) throw QueueErrors::INDEX_OUT_OF_BOUNDS;

				// Get the smallest and biggest index

				size_t small_index = std::min(index_1, index_2);
				size_t big_index = std::max(index_1, index_2);

				// Get the node at index_1 and index_2

				QueueNode<type> *node_1 = first;

				for (size_t i = 0; i < small_index; i++) {
					node_1 = node_1->next;
				}

				QueueNode<type> *node_2 = node_1;

				for (size_t i = 0; i < big_index - small_index; i++) {
					node_2 = node_2->next;
				}

				// Swap their values

				type node_1_value = node_1->value;
				node_1->value = node_2->value;
				node_2->value = node_1_value;
			}

			/**
			 *  @brief  Places a value at the end of the Queue.
			 *  @param  value  A reference to the value to push.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			void push(const type& value)
			{
				QueueNode<type> *new_node = new QueueNode<type>(value);

				if (current_element_count == 0) {
					first = new_node;
					last = new_node;
				} else {
					last->next = new_node;
					last = new_node;
				}

				current_element_count++;
			}

			/**
			 *  @brief  Places a value before at the start of the Queue.
			 *  @param  value  A reference to the value to add to the Queue.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			void prepend(const type& value)
			{
				QueueNode<type> *new_node = new QueueNode<type>(value);

				if (current_element_count == 0) {
					first = new_node;
					last = new_node;
				} else {
					new_node->next = first;
					first = new_node;
				}

				current_element_count++;
			}

			/**
			 *  @brief  Places a value to the end of the Queue.
			 *  @param  value  A reference to the value to push.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			void operator+=(const type& value) { push(value); }

			/**
			 *  @brief  Places another Queue at the end of this Queue.
			 *  The values will be transfered to this Queue, and the other
			 *  Queue will be reset.
			 *  @param  other_queue  The other Queue.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			void attach(Queue<type>& other_queue) {
				if (this == &other_queue) return;

				if (other_queue.size()) {
					last->next = other_queue.first;
					last = other_queue.last;

					current_element_count += other_queue.current_element_count;

					other_queue.current_element_count = 0;
					other_queue.first = NULL;
					other_queue.last = NULL;
				}
			}

			/**
			 *  @brief  Places another Queue at the end of this Queue.
			 *  The values will be transfered to this Queue, and the other
			 *  Queue will be reset.
			 *  @param  other_queue  The other Queue.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			void operator+=(Queue<type>& other_queue) { attach(other_queue); }

			/**
			 *  @brief  Places another Queue at the beginning of this Queue.
			 *  The values will be transfered to this Queue, and the other
			 *  Queue will be reset.
			 *  @param  other_queue  The other queue.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			void precede(Queue<type>& other_queue) {
				if (this == &other_queue) return;

				if (other_queue.size()) {
					QueueNode<type> *old_first = first;

					first = other_queue.first;
					other_queue.last->next = old_first;

					current_element_count += other_queue.current_element_count;

					other_queue.current_element_count = 0;
					other_queue.first = NULL;
					other_queue.last = NULL;
				}
			}

			/**
			 *  @brief  Deletes the first element of the Queue and returns it.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			type pop()
			{
				if (current_element_count == 0) throw QueueErrors::POP_EMPTY_QUEUE;

				QueueNode<type> *new_first = first->next;
				type value = first->value;

				delete first;
				current_element_count--;
				first = new_first;

				return value;
			}
	};
};

#endif