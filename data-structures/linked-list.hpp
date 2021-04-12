#ifndef FLOW_LINKED_LIST_HEADER
#define FLOW_LINKED_LIST_HEADER

#include <bits/stdc++.h>

#include "dynamic-array.hpp"
#include "../iterators/node-list-iterator.hpp"

namespace flow_linked_list_tools {
	template <typename type>
	struct Node {
		Node<type> *prev;
		Node<type> *next;
		type value;

		Node() {}
		Node(const type& value) : value(value) {}
		Node(type&& value) : value(std::move(value)) {}
	};

	enum class LinkedListErrors {
		INDEX_OUT_OF_BOUNDS,
		EXTRACT_FROM_EMPTY_LIST,
		ATTACH_LIST_TO_ITSELF
	};
};

namespace flow {
	using namespace flow_linked_list_tools;

	template <typename type>
	class LinkedList {
		protected:
			size_t cur_size;
			Node<type> head;
			Node<type> tail;

			Node<type>& get_node(size_t i)
			{
				if (i >= cur_size) throw LinkedListErrors::INDEX_OUT_OF_BOUNDS;

				Node<type> *node;

				// If i is in the last half of the LinkedList, traverse backwards

				if (i > cur_size / 2) {
					node = tail.prev;
					for (size_t j = cur_size - 1; j != i; j--) node = node->prev;
					return *node;
				}

				// If i is i the first half of the LinkedList, traverse forwards

				else {
					node = head.next;
					for (size_t j = 0; j != i; j++) node = node->next;
					return *node;
				}
			}

		public:
			/**
			 *  @brief  Creates a LinkedList with no elements.
			 */
			LinkedList() : cur_size(0)
			{
				head.next = &tail;
				head.prev = NULL;
				tail.next = NULL;
				tail.prev = &head;
			}

			/**
			 *  @brief  Deletes all nodes on the LinkedList.
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			~LinkedList()
			{
				Node<type> *node = head.next;

				for (size_t i = 0; i < cur_size; i++) {
					Node<type> *next_node = node->next;
					delete node;
					node = next_node;
				}
			}

			/**
			 *  @brief  Returns a read-only reference to the head of the LinkedList.
			 */
			const type& front() const
			{
				return head.next->value;
			}

			/**
			 *  @brief  Returns a read/write reference to the head of the LinkedList.
			 */
			type& front()
			{
				return head.next->value;
			}

			/**
			 *  @brief  Returns a read-only reference to the tail of the LinkedList.
			 */
			const type& back() const
			{
				return tail.prev->value;
			}

			/**
			 *  @brief  Returns a read/write reference to the tail of the LinkedList.
			 */
			type& back()
			{
				return tail.prev->value;
			}

			/**
			 *  @brief  Returns the current number of elements on the LinkedList.
			 */
			size_t size() const
			{
				return cur_size;
			}

			/**
			 *  @brief  Read/write iterator for the data in the LinkedList nodes.
			 *  Iteration is done in-order.
			 */
			using Iterator = NodeListIterator<type, Node<type>, false, true>;

			/**
			 *  @brief  Read-only iterator for the data in the LinkedList nodes.
			 *  Iteration is done in-order.
			 */
			using ConstIterator = NodeListIterator<type, Node<type>, true, true>;

			/**
			 *  @brief  Returns a read/write iterator that points to the first
			 *  element of the LinkedList. Iteration is done in-order.
			 */
			Iterator begin()
			{
				return Iterator(head.next);
			}

			/**
			 *  @brief  Returns a read/write iterator that points to one past
			 *  the last element of the LinkedList. Iteration is done in-order.
			 */
			Iterator end()
			{
				return Iterator(&tail);
			}

			/**
			 *  @brief  Returns a reverse read/write iterator that points to the
			 *  last element of the LinkedList. Iteration is done in-order.
			 */
			Iterator rbegin()
			{
				return Iterator(tail.prev);
			}

			/**
			 *  @brief  Returns a reverse read/write iterator that points to one
			 *  before the first element of the LinkedList.
			 *  Iteration is done in-order.
			 */
			Iterator rend()
			{
				return Iterator(&head);
			}

			/**
			 *  @brief  Returns a read-only iterator that points to the first
			 *  element of the LinkedList. Iteration is done in-order.
			 */
			ConstIterator cbegin() const
			{
				return ConstIterator(head.next);
			}

			/**
			 *  @brief  Returns a read-only iterator that points to one past
			 *  the last element of the LinkedList. Iteration is done in-order.
			 */
			ConstIterator cend() const
			{
				return ConstIterator(&tail);
			}

			/**
			 *  @brief  Returns a reverse read-only iterator that points to the
			 *  last element of the LinkedList. Iteration is done in-order.
			 */
			ConstIterator crbegin() const
			{
				return ConstIterator(tail.prev);
			}

			/**
			 *  @brief  Returns a reverse read-only iterator that points to one
			 *  before the first element of the LinkedList.
			 *  Iteration is done in-order.
			 */
			ConstIterator crend() const
			{
				return ConstIterator(&head);
			}

			/**
			 *  @brief  Returns a read-only reference to the element at the i-th index
			 *  on the LinkedList.
			 *  @param  i  i
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			const type& operator[](size_t i) const
			{
				return get_node(i).value;
			}

			/**
			 *  @brief  Returns a read/write reference to the element at the i-th index
			 *  on the LinkedList.
			 *  @param  i  i
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			type& operator[](size_t i)
			{
				return get_node(i).value;
			}

			/**
			 *  @brief  Returns the index of the first occurrence of a given value.
			 *  Returns -1 if the value was not found.
			 *  @param  value  The value to find.
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			ssize_t first_index_of(const type& value) const
			{
				Node<type> *node = head.next;

				for (size_t i = 0; i < cur_size; i++) {
					if (node->value == value) return i;
					node = node->next;
				}

				return -1;
			}

			/**
			 *  @brief  Returns the index of the last occurrence of a given value.
			 *  Returns -1 if the value was not found.
			 *  @param  value  The value to find.
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			ssize_t last_index_of(const type& value) const
			{
				Node<type> *node = tail.prev;

				for (size_t i = 0; i < cur_size; i++) {
					if (node->value == value) return i;
					node = node->prev;
				}

				return -1;
			}

			/**
			 *  @brief  Returns the indices of all occurrences of a given value.
			 *  The indices are returned in-order from smallest to biggest.
			 *  @param  value  The value to find.
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(n), n = found indices
			 */
			DynamicArray<size_t> indices_of(const type& value) const
			{
				DynamicArray<size_t> indices;
				Node<type> *node = head.next;

				for (size_t i = 0; i < cur_size; i++) {
					if (node->value == value) indices.append(i);
					node = node->next;
				}

				return indices;
			}

			/**
			 *  @brief  Swaps the values at two indices	of the LinkedList.
			 *  @param  index_1  The first index.
			 *  @param  index_2  The second index.
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			void swap_indices(size_t index_1, size_t index_2)
			{
				if (index_1 == index_2) return;
				if (index_1 > cur_size || index_2 > cur_size)
					throw LinkedListErrors::INDEX_OUT_OF_BOUNDS;

				Node<type> *node_1;
				Node<type> *node_2;

				// Get the smallest and biggest index

				size_t small_index = std::min(index_1, index_2);
				size_t big_index = std::max(index_1, index_2);

				// Collect nodes

				#define START_TO_NODE_1   small_index
				#define NODE_2_TO_END     cur_size - 1 - big_index
				#define DISTANCE          big_index - small_index
				#define COLLECT_FORWARDS  START_TO_NODE_1 + DISTANCE
				#define COLLECT_BACKWARDS DISTANCE + NODE_2_TO_END
				#define COLLECT_FW_BW     START_TO_NODE_1 + NODE_2_TO_END

				// Collect forwards

				if (
					COLLECT_FORWARDS < COLLECT_BACKWARDS
					&& COLLECT_FORWARDS < COLLECT_FW_BW
				) {
					node_1 = head.next;

					for (size_t i = 0; i < START_TO_NODE_1; i++) {
						node_1 = node_1->next;
					}

					node_2 = node_1;

					for (size_t i = 0; i < DISTANCE; i++) {
						node_2 = node_2->next;
					}
				}

				// Collect backwards

				else if (
					COLLECT_BACKWARDS < COLLECT_FORWARDS
					&& COLLECT_BACKWARDS < COLLECT_FW_BW
				) {
					node_2 = tail.prev;

					for (size_t i = 0; i < NODE_2_TO_END; i++) {
						node_2 = node_2->prev;
					}

					node_1 = node_2;

					for (size_t i = 0; i < DISTANCE; i++) {
						node_1 = node_1->prev;
					}
				}

				// Collect forwards/backwards

				else {
					node_1 = head.next;

					for (size_t i = 0; i < START_TO_NODE_1; i++) {
						node_1 = node_1->next;
					}

					node_2 = tail.prev;

					for (size_t i = 0; i < NODE_2_TO_END; i++) {
						node_2 = node_2->prev;
					}
				}

				// Swap their values

				type node_1_value = node_1->value;
				node_1->value = node_2->value;
				node_2->value = node_1_value;

				#undef COLLECT_FW_BW
				#undef COLLECT_FORWARDS
				#undef COLLECT_BACKWARDS
				#undef START_TO_NODE_1
				#undef NODE_2_TO_END
				#undef DISTANCE
			}

			/**
			 *  @brief  Swaps the values at two iterators of the LinkedList.
			 *  @param  iterator_1  An iterator to the first element.
			 *  @param  iterator_2  An iterator to the second element.
			 */
			void swap(Iterator& iterator_1, Iterator& iterator_2)
			{
				type temp = *iterator_1;
				*iterator_1 = *iterator_2;
				*iterator_2 = temp;
			}

			/**
			 *  @brief  Inserts an element at the end of the LinkedList.
			 *  @param  value  The value to place.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			void append(const type& value)
			{
				Node<type> *new_node = new Node<type>(value);

				new_node->next = &tail;
				new_node->prev = tail.prev;
				tail.prev->next = new_node;
				tail.prev = new_node;

				cur_size++;
			}

			/**
			 *  @brief  Inserts an element at the end of the LinkedList.
			 *  @param  value  The value to place.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			void append(type&& value)
			{
				Node<type> *new_node = new Node<type>(std::move(value));

				new_node->next = &tail;
				new_node->prev = tail.prev;
				tail.prev->next = new_node;
				tail.prev = new_node;

				cur_size++;
			}

			/**
			 *  @brief  Inserts an element at the end of the LinkedList.
			 *  @param  value  The value to place.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			void operator+=(const type& value) { append(value); }

			/**
			 *  @brief  Inserts an element at the end of the LinkedList.
			 *  @param  value  The value to place.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			void operator+=(type&& value) { append(std::move(value)); }

			/**
			 *  @brief  Deletes the last element of the LinkedList. The second
			 *  last element of the LinkedList will become the new tail.
			 *  @returns The value of the element that was popped off.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			type extract_rear()
			{
				if (cur_size == 0) throw LinkedListErrors::EXTRACT_FROM_EMPTY_LIST;

				type value = tail.prev->value;
				Node<type> *new_rear = tail.prev->prev;

				delete tail.prev;
				cur_size--;

				tail.prev = new_rear;
				new_rear->next = &tail;
			}

			/**
			 *  @brief  Inserts an element at the beginning of the LinkedList.
			 *  @param  value  The value to place.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			void prepend(const type& value)
			{
				Node<type> *new_node = new Node<type>(value);

				new_node->prev = &head;
				new_node->next = head.next;
				head.next->prev = new_node;
				head.next = new_node;

				cur_size++;
			}

			/**
			 *  @brief  Inserts an element at the beginning of the LinkedList.
			 *  @param  value  The value to place.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			void prepend(type&& value)
			{
				Node<type> *new_node = new Node<type>(std::move(value));

				new_node->prev = &head;
				new_node->next = head.next;
				head.next->prev = new_node;
				head.next = new_node;

				cur_size++;
			}

			/**
			 *  @brief  Deletes the first element of the LinkedList. The
			 *  second element of the LinkedList will become the new head.
			 *  @returns The value of the element that was popped off.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			type extract_front()
			{
				if (cur_size == 0) throw LinkedListErrors::EXTRACT_FROM_EMPTY_LIST;

				type value = head.next->value;
				Node<type> *new_front = head.next->next;

				delete head.next;
				cur_size--;

				head.next = new_front;
				new_front->prev = &head;
			}

			/**
			 *  @brief  Inserts an element at a specific index of the LinkedList.
			 *  The new value is placed between the values already on the LinkedList.
			 *  @param  i The index at which to place the value.
			 *  @param  value  The value to place.
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			void insert(size_t i, const type& value)
			{
				Node<type> *new_node = new Node<type>(value);

				Node<type> next_node = get_node(i);
				Node<type> *prev_node = next_node.prev;

				prev_node->next = new_node;
				new_node->prev = prev_node;
				next_node.prev = new_node;
				new_node->next = &next_node;

				cur_size++;
			}

			/**
			 *  @brief  Inserts an element at a specific index of the LinkedList.
			 *  The new value is placed between the values already on the LinkedList.
			 *  @param  i The index at which to place the value.
			 *  @param  value  The value to place.
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			void insert(size_t i, type&& value)
			{
				Node<type> *new_node = new Node<type>(std::move(value));

				Node<type> next_node = get_node(i);
				Node<type> *prev_node = next_node.prev;

				prev_node->next = new_node;
				new_node->prev = prev_node;
				next_node.prev = new_node;
				new_node->next = &next_node;

				cur_size++;
			}

			/**
			 *  @brief  Inserts an element right after a given iterator.
			 *  @param  it  The iterator to the value before the new value.
			 *  @param  value  The value to place.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			void insert(Iterator& it, const type& value)
			{
				Node<type> *new_node = new Node<type>(value);

				Node<type>& prev_node = it.get_node();
				Node<type> *next_node = prev_node.next;

				prev_node.next = new_node;
				new_node->prev = &prev_node;
				next_node->prev = new_node;
				new_node->next = next_node;

				cur_size++;
			}

			/**
			 *  @brief  Inserts an element right after a given iterator.
			 *  @param  it  The iterator to the value before the new value.
			 *  @param  value  The value to place.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			void insert(Iterator& it, type&& value)
			{
				Node<type> *new_node = new Node<type>(std::move(value));

				Node<type>& prev_node = it.get_node();
				Node<type> *next_node = prev_node.next;

				prev_node.next = new_node;
				new_node->prev = &prev_node;
				next_node->prev = new_node;
				new_node->next = next_node;

				cur_size++;
			}

			/**
			 *  @brief  Deletes an element at a specific index of the LinkedList.
			 *  The element after the deleted element will be placed right after
			 *  the element before the deleted element.
			 *  @param  i  The index at which to delete the value.
			 *  @returns  The value of the deleted element.
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			type remove(size_t i)
			{
				if (cur_size == 0)
					throw LinkedListErrors::EXTRACT_FROM_EMPTY_LIST;

				Node<type>& next_node = get_node(i);
				Node<type> *prev_node = next_node.prev->prev;
				type value = next_node.prev->value;

				delete next_node.prev;
				cur_size--;

				next_node.prev = prev_node;
				prev_node->next = &next_node;

				return value;
			}

			/**
			 *  @brief  Deletes an element pointed by an iterator.
			 *  The element after the deleted element will be placed right after
			 *  the element before the deleted element.
			 *  @param  it  An iterator to the element to delete.
			 *  @returns  The value of the deleted element.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			type remove(Iterator& it)
			{
				if (cur_size == 0)
					throw LinkedListErrors::EXTRACT_FROM_EMPTY_LIST;

				Node<type>& prev_node = it.get_node();
				Node<type> *next_node = prev_node.next->next;
				type value = next_node->prev->value;

				delete next_node->prev;
				cur_size--;

				next_node->prev = &prev_node;
				prev_node.next = next_node;

				return value;
			}

			/**
			 *  @brief  Places another LinkedList at the end of
			 *  this LinkedList.
			 *  The values will be transfered to this LinkedList,
			 *  and the other LinkedList will be reset.
			 *  @param  other  The other LinkedList.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			void attach(LinkedList<type>& other)
			{
				if (this == &other)
					throw LinkedListErrors::ATTACH_LIST_TO_ITSELF;

				if (other.cur_size) {
					tail.prev->next = other.head.next;
					other.head.next->prev = tail.prev;
					tail.prev = other.tail.prev;

					cur_size += other.cur_size;

					other.cur_size = 0;
					other.head.next = NULL;
					other.tail.prev = NULL;
				}
			}

			/**
			 *  @brief  Places another LinkedList at the end of
			 *  this LinkedList.
			 *  The values will be transfered to this LinkedList,
			 *  and the other LinkedList will be reset.
			 *  @param  other  The other LinkedList.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			void operator+=(LinkedList<type>& other) { attach(other); }

			/**
			 *  @brief  Places another LinkedList at the beginning of
			 *  this LinkedList.
			 *  The values will be transfered to this LinkedList,
			 *  and the other LinkedList will be reset.
			 *  @param  other  The other LinkedList.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			void precede(LinkedList<type>& other)
			{
				if (this == &other)
					throw LinkedListErrors::ATTACH_LIST_TO_ITSELF;

				if (other.cur_size) {
					head.next->prev = other.tail.prev;
					other.tail.prev->next = head.next;
					head.next = other.head.next;

					cur_size += other.cur_size;

					other.cur_size = 0;
					other.head.next = NULL;
					other.tail.prev = NULL;
				}
			}
	};
};

#endif