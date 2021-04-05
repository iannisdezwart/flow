#ifndef FLOW_LINKED_LIST_HEADER
#define FLOW_LINKED_LIST_HEADER

#include <bits/stdc++.h>

#include "dynamic-array.hpp"

namespace flow_linked_list_tools {
	template <typename type, bool Doubly>
	class LinkedListNode;

	/**
	 *  @brief  Doubly LinkedListNode
	 */
	template <typename type>
	class LinkedListNode<type, true> {
		public:
			type value;
			LinkedListNode<type, true> *next;
			LinkedListNode<type, true> *prev;

			LinkedListNode(const type& value, LinkedListNode<type, true> *prev = NULL,
				LinkedListNode<type, true> *next = NULL)
					: value(value), prev(prev), next(next) {}
	};

	/**
	 *  @brief  Singly LinkedListNode
	 */
	template <typename type>
	class LinkedListNode<type, false> {
		public:
			type value;
			LinkedListNode<type, false> *next;

			LinkedListNode(const type& value,	LinkedListNode<type, false> *next = NULL)
				: value(value), next(next) {}
	};

	enum class LinkedListErrors {
		INDEX_OUT_OF_BOUNDS,
		EXTRACT_FROM_EMPTY_LINKED_LIST,
		ATTACH_LINKED_LIST_TO_ITSELF
	};

	template <typename type, bool Doubly = false>
	class LinkedListBase;

	template <typename type>
	class LinkedListBase<type, true> {
		protected:
			size_t cur_size;
			LinkedListNode<type, true> *head;
			LinkedListNode<type, true> *tail;

		public:
			/**
			 *  @brief  Creates LinkedList with zero elements.
			 */
			LinkedListBase() : cur_size(0), head(NULL), tail(NULL) {}
	};

	template <typename type>
	class LinkedListBase<type, false> {
		protected:
			size_t cur_size;
			LinkedListNode<type, false> *head;

		public:
			/**
			 *  @brief  Creates LinkedList with zero elements.
			 */
			LinkedListBase() : cur_size(0), head(NULL) {}
	};
};

namespace flow {
	using namespace flow_linked_list_tools;

	/**
	 *  @brief  LinkedList implementation.
	 *  @tparam  type  The type of values to store on this LinkedList.
	 *  @tparam  Doubly  If true, the tail element will be available for
	 *  instant access and allows for some optimisations. Uses more memory.
	 *  Defaults to false.
	 */
	template <typename type, bool Doubly = false>
	class LinkedList : public LinkedListBase<type, Doubly> {
		protected:
			/**
			 *  @brief  Returns a read-write reference to the node at position i.
			 *  @param  i  i
			 */
			template <bool T = true>
			typename std::enable_if<T && Doubly, LinkedListNode<type, true>&>::type
			/* LinkedListNode<type, true>& */ get_node(size_t i)
			{
				if (i >= this->cur_size) throw LinkedListErrors::INDEX_OUT_OF_BOUNDS;

				LinkedListNode<type, true> *node;

				// If i is in the last half of the LinkedList, traverse backwards

				if (i > this->cur_size / 2) {
					node = this->tail;
					for (size_t j = this->cur_size - 1; j != i; j--) node = node->prev;
					return *node;
				}

				// If i is i the first half of the LinkedList, traverse forwards

				else {
					node = this->head;
					for (size_t j = 0; j != i; j++) node = node->next;
					return *node;
				}
			}

			/**
			 *  @brief  Returns a read-write reference to the node at position i.
			 *  @param  i  i
			 */
			template <bool T = true>
			typename std::enable_if<T && !Doubly, LinkedListNode<type, false>&>::type
			/* LinkedListNode<type, false>& */ get_node(size_t i)
			{
				if (i >= this->cur_size) throw LinkedListErrors::INDEX_OUT_OF_BOUNDS;

				LinkedListNode<type, false> *node = this->head;
				for (size_t j = 0; j != i; j++) node = node->next;
				return *node;
			}

		public:
			/**
			 *  @brief  Deletes all LinkedListNodes on the LinkedList.
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			~LinkedList()
			{
				LinkedListNode<type, Doubly> *node = this->head;

				for (size_t i = 0; i < this->cur_size; i++) {
					LinkedListNode<type, Doubly> *next_node = node->next;
					delete node;
					node = next_node;
				}
			}

			/**
			 *  @brief  Returns a read-only reference to the head of the LinkedList.
			 */
			const type& front() const { return this->head->value; }

			/**
			 *  @brief  Returns a read/write reference to the head of the LinkedList.
			 */
			type& front() { return this->head->value; }

			/**
			 *  @brief  Returns a read-only reference to the tail of the LinkedList.
			 */
			template <bool T = true>
			typename std::enable_if<T && Doubly, const type&>::type
			/* const type & */ back() const { return this->tail->value; }

			/**
			 *  @brief  Returns a read/write reference to the tail of the LinkedList.
			 */
			template <bool T = true>
			typename std::enable_if<T && Doubly, type&>::type
			/* type & */ back() { return this->tail->value; }

			/**
			 *  @brief  Returns the current number of elements on the LinkedList.
			 */
			size_t size() const { return this->cur_size; }

			class Iterator {
				public:
					using iterator_category = std::forward_iterator_tag;
					using difference_type = std::ptrdiff_t;

					Iterator(LinkedListNode<type, Doubly> *node)
						: node(node) {}

					const type& operator*() const
					{
						return node->value;
					}

					type operator->()
					{
						return node->value;
					}

					Iterator& /* prefix */ operator++()
					{
						node = node->next;
						return *this;
					}

					Iterator /* postfix */ operator++(int)
					{
						Iterator old_it = *this;
						node = node->next;
						return old_it;
					}

					template <bool T = true>
					typename std::enable_if<T && Doubly, Iterator>::type
					/* Iterator */ /* prefix */ operator--()
					{
						node = node->prev;
						return *this;
					}

					template <bool T = true>
					typename std::enable_if<T && Doubly, Iterator>::type
					/* Iterator */ /* postfix */ operator--(int)
					{
						Iterator old_it = *this;
						node = node->prev;
						return old_it;
					}

					bool operator==(const Iterator& other)
					{
						return node == other.node;
					}

					bool operator!=(const Iterator& other)
					{
						return node != other.node;
					}

				private:
					LinkedListNode<type, Doubly> *node;
			};

			Iterator begin()
			{
				return Iterator(this->head);
			}

			Iterator end()
			{
				return Iterator(NULL);
			}

			template <bool T = true>
			typename std::enable_if<T && Doubly, Iterator>::type
			/* Iterator */ rbegin()
			{
				return Iterator(this->tail);
			}

			template <bool T = true>
			typename std::enable_if<T && Doubly, Iterator>::type
			/* Iterator */ rend()
			{
				return Iterator(NULL);
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
				LinkedListNode<type, Doubly> *node = this->head;

				for (size_t i = 0; i < this->cur_size; i++) {
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
			template <bool T = true>
			typename std::enable_if<T && Doubly, ssize_t>::type
			/* ssize_t */ last_index_of(const type& value) const
			{
				LinkedListNode<type, Doubly> *node = this->tail;

				for (size_t i = this->cur_size; i != 0; i--) {
					if (node->value == value) return i - 1;
					node = node->prev;
				}

				return -1;
			}

			/**
			 *  @brief  Returns the indices of all occurrences of a given value.
			 *  @param  value  The value to find.
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(n), n = found indices
			 */
			DynamicArray<size_t> indices_of(const type& value) const
			{
				DynamicArray<size_t> indices;
				LinkedListNode<type, Doubly> *node = this->head;

				for (size_t i = 0; i < this->cur_size; i++) {
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
			template <bool T = true>
			typename std::enable_if<T && Doubly, void>::type
			/* void */ swap_indices(size_t index_1, size_t index_2)
			{
				if (index_1 == index_2) return;
				if (index_1 > this->cur_size) throw LinkedListErrors::INDEX_OUT_OF_BOUNDS;
				if (index_2 > this->cur_size) throw LinkedListErrors::INDEX_OUT_OF_BOUNDS;

				LinkedListNode<type, Doubly> *node_1;
				LinkedListNode<type, Doubly> *node_2;

				// Get the smallest and biggest index

				size_t small_index = std::min(index_1, index_2);
				size_t big_index = std::max(index_1, index_2);

				// Collect nodes

				#define START_TO_NODE_1   small_index
				#define NODE_2_TO_END     this->cur_size - 1 - big_index
				#define DISTANCE          big_index - small_index
				#define COLLECT_FORWARDS  START_TO_NODE_1 + DISTANCE
				#define COLLECT_BACKWARDS DISTANCE + NODE_2_TO_END
				#define COLLECT_FW_BW     START_TO_NODE_1 + NODE_2_TO_END

				// Collect forwards

				if (
					COLLECT_FORWARDS < COLLECT_BACKWARDS
					&& COLLECT_FORWARDS < COLLECT_FW_BW
				) {
					node_1 = this->head;

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
					node_2 = this->tail;

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
					node_1 = this->head;

					for (size_t i = 0; i < START_TO_NODE_1; i++) {
						node_1 = node_1->next;
					}

					node_2 = this->tail;

					for (size_t i = 0; i < NODE_2_TO_END; i++) {
						node_2 = node_2->prev;
					}
				}

				#undef COLLECT_FW_BW
				#undef COLLECT_FORWARDS
				#undef COLLECT_BACKWARDS
				#undef START_TO_NODE_1
				#undef NODE_2_TO_END
				#undef DISTANCE

				// Swap their values

				type node_1_value = node_1->value;
				node_1->value = node_2->value;
				node_2->value = node_1_value;
			}

			/**
			 *  @brief  Swaps the values at two indices	of the LinkedList.
			 *  @param  index_1  The first index.
			 *  @param  index_2  The second index.
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			template <bool T = true>
			typename std::enable_if<T && !Doubly, void>::type
			/* void */ swap_indices(size_t index_1, size_t index_2)
			{
				if (index_1 == index_2) return;
				if (index_1 > this->cur_size) throw LinkedListErrors::INDEX_OUT_OF_BOUNDS;
				if (index_2 > this->cur_size) throw LinkedListErrors::INDEX_OUT_OF_BOUNDS;

				LinkedListNode<type, Doubly> *node_1;
				LinkedListNode<type, Doubly> *node_2;

				// Get the smallest and biggest index

				size_t small_index = std::min(index_1, index_2);
				size_t big_index = std::max(index_1, index_2);

				// Collect nodes

				node_1 = this->head;

				for (size_t i = 0; i < small_index; i++) {
					node_1 = node_1->next;
				}

				node_2 = node_1;

				for (size_t i = 0; i < big_index - small_index; i++) {
					node_2 = node_2->next;
				}

				// Swap their values

				type node_1_value = node_1->value;
				node_1->value = node_2->value;
				node_2->value = node_1_value;
			}

			/**
			 *  @brief  Inserts an element at the end of the LinkedList.
			 *  @param  value  The value to place.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			template <bool T = true>
			typename std::enable_if<T && Doubly, void>::type
			/* void */ append(const type& value)
			{
				LinkedListNode<type, true> *new_node =
					new LinkedListNode<type, true>(value);

				if (this->head == NULL) {
					this->head = new_node;
					this->tail = new_node;
				} else {
					this->tail->next = new_node;
					new_node->prev = this->tail;
					this->tail = new_node;
				}

				this->cur_size++;
			}

			/**
			 *  @brief  Inserts an element at the end of the LinkedList.
			 *  @param  value  The value to place.
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			template <bool T = true>
			typename std::enable_if<T && !Doubly, void>::type
			/* void */ append(const type& value)
			{
				LinkedListNode<type, false> *new_node =
					new LinkedListNode<type, false>(value);

				if (this->head == NULL) {
					this->head = new_node;
				} else {
					LinkedListNode<type, false> *node = this->head;
					while (node->next != NULL) node = node->next;
					node->next = new_node;
				}

				this->cur_size++;
			}

			/**
			 *  @brief  Inserts a value at the end of this LinkedList.
			 */
			void operator+=(const type& value) { append(value); }

			/**
			 *  @brief  Deletes the last element of the LinkedList.
			 *  The second last element of the LinkedList will become the new tail.
			 *  @returns The value of the element that was popped off.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			template <bool T = true>
			typename std::enable_if<T && Doubly, type>::type
			/* type */ extract_rear()
			{
				if (this->cur_size == 0) throw LinkedListErrors::EXTRACT_FROM_EMPTY_LINKED_LIST;

				type value = this->tail->value;
				LinkedListNode<type, Doubly> *new_tail = this->tail->prev;

				delete this->tail;
				this->cur_size--;
				this->tail = new_tail;

				return value;
			}

			/**
			 *  @brief  Deletes the last element of the LinkedList.
			 *  The second last element of the LinkedList will become the new tail.
			 *  @returns The value of the element that was popped off.
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			template <bool T = true>
			typename std::enable_if<T && !Doubly, type>::type
			/* type */ extract_rear()
			{
				if (this->cur_size == 0) throw LinkedListErrors::EXTRACT_FROM_EMPTY_LINKED_LIST;

				LinkedListNode<type, false> *node = this->head;
				while (node->next != NULL) node = node->next;
				type value = node->value;

				delete node;
				this->cur_size--;

				return value;
			}

			/**
			 *  @brief  Inserts an element at the beginning of the LinkedList.
			 *  @param  value  The value to place.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			template <bool T = true>
			typename std::enable_if<T && Doubly, void>::type
			/* void */ prepend(const type& value)
			{
				LinkedListNode<type, Doubly> *new_node =
					new LinkedListNode<type, Doubly>(value);

				if (this->cur_size == 0) {
					this->head = new_node;
					this->tail = new_node;
				} else {
					new_node->next = this->head;
					this->head = new_node;
				}

				this->cur_size++;
			}

			/**
			 *  @brief  Inserts an element at the beginning of the LinkedList.
			 *  @param  value  The value to place.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			template <bool T = true>
			typename std::enable_if<T && !Doubly, void>::type
			/* void */ prepend(const type& value)
			{
				LinkedListNode<type, Doubly> *new_node =
					new LinkedListNode<type, Doubly>(value);

				new_node->next = this->head;
				this->head = new_node;

				this->cur_size++;
			}

			/**
			 *  @brief  Deletes the first element of the LinkedList.
			 *  The second element of the LinkedList will become the new head.
			 *  @returns The value of the element that was popped off.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			type extract_front()
			{
				if (this->cur_size == 0) throw LinkedListErrors::EXTRACT_FROM_EMPTY_LINKED_LIST;

				type value = this->head->value;
				LinkedListNode<type, Doubly> *new_head = this->head->next;

				delete this->head;
				this->cur_size--;
				this->head = new_head;

				return value;
			}

			/**
			 *  @brief  Inserts an element at a specific index of the LinkedList.
			 *  The new value is placed between the values already on the LinkedList.
			 *  @param  i The index at which to place the value.
			 *  @param  value  The value to place.
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			template <bool T = true>
			typename std::enable_if<T && Doubly, void>::type
			/* void */ insert(size_t i, const type& value)
			{
				if (i == 0) prepend(value);
				if (i == this->cur_size - 1) append(value);

				LinkedListNode<type, true>& prev_node = get_node(i - 1);
				LinkedListNode<type, true> *next_node = prev_node.next;

				LinkedListNode<type, true> *new_node =
					new LinkedListNode<type, true>(value);

				prev_node.next = new_node;
				new_node->next = next_node;
				new_node->prev = &prev_node;
				next_node->prev = new_node;

				this->cur_size++;
			}

			/**
			 *  @brief  Inserts an element at a specific index of the LinkedList.
			 *  The new value is placed between the values already on the LinkedList.
			 *  @param  i The index at which to place the value.
			 *  @param  value  The value to place.
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			template <bool T = true>
			typename std::enable_if<T && !Doubly, void>::type
			/* void */ insert(size_t i, const type& value)
			{
				if (i == 0) prepend(value);
				if (i == this->cur_size - 1) append(value);

				LinkedListNode<type, false>& prev_node = get_node(i - 1);
				LinkedListNode<type, false> *next_node = prev_node.next;

				prev_node.next = new LinkedListNode<type, false>(value);
				prev_node.next->next = next_node;

				this->cur_size++;
			}

			/**
			 *  @brief  Deletes an element at a specific index of the LinkedList.
			 *  The element after the deleted element will be placed right after
			 *  the element before the deleted element.
			 *  @param  i  The index at which to delete the value.
			 *  @returns  The value of the deleted element
			 *  @note  Runtime: O(n)
			 *  @note  Memory: O(1)
			 */
			type remove(size_t i)
			{
				if (i == 0) return extract_front();
				if (i == this->cur_size - 1) return extract_rear();

				if (this->cur_size == 0)
					throw LinkedListErrors::EXTRACT_FROM_EMPTY_LINKED_LIST;

				LinkedListNode<type, Doubly>& prev_node = get_node(i - 1);
				LinkedListNode<type, Doubly> *next_node = prev_node.next->next;
				type value = prev_node.next->value;

				delete prev_node.next;
				this->cur_size--;
				prev_node.next = next_node;

				return value;
			}

			/**
			 *  @brief  Places another LinkedList at the end of this LinkedList.
			 *  The values will be transfered to this LinkedList, and the other
			 *  LinkedList will be reset.
			 *  @param  other_linked_list  The other LinkedList.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			template <bool T = true>
			typename std::enable_if<T && Doubly, void>::type
			/* void */ attach(LinkedList<type, true>& other_linked_list)
			{
				if (this == &other_linked_list)
					throw LinkedListErrors::ATTACH_LINKED_LIST_TO_ITSELF;

				if (other_linked_list.size()) {
					this->tail->next = other_linked_list.head;
					other_linked_list.head->prev = this->tail;
					this->tail = other_linked_list.tail;

					this->cur_size += other_linked_list.cur_size;

					other_linked_list.cur_size = 0;
					other_linked_list.head = NULL;
					other_linked_list.tail = NULL;
				}
			}

			/**
			 *  @brief  Places another LinkedList at the end of this LinkedList.
			 *  The values will be transfered to this LinkedList, and the other
			 *  LinkedList will be reset.
			 *  @param  other_linked_list  The other LinkedList.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			template <bool T = true>
			typename std::enable_if<T && Doubly, void>::type
			/* void */ operator+=(LinkedList<type, true>& other_linked_list)
			{
				attach(other_linked_list);
			}

			/**
			 *  @brief  Places another LinkedList at the beginning of this LinkedList.
			 *  The values will be transfered to this LinkedList, and the other
			 *  LinkedList will be reset.
			 *  @param  other_linked_list  The other LinkedList.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			template <bool T = true>
			typename std::enable_if<T && Doubly, void>::type
			/* void */ precede(LinkedList<type, true>& other_linked_list)
			{
				if (this == &other_linked_list)
					throw LinkedListErrors::ATTACH_LINKED_LIST_TO_ITSELF;

				if (other_linked_list.size()) {
					LinkedListNode<type, true> *old_head = this->head;

					this->head = other_linked_list.head;
					other_linked_list.tail->next = old_head;
					old_head->prev = other_linked_list.tail;

					this->cur_size += other_linked_list.cur_size;

					other_linked_list.cur_size = 0;
					other_linked_list.head = NULL;
					other_linked_list.tail = NULL;
				}
			}
	};

	/**
	 *  @brief  Doubly LinkedList implementation.
	 *  This is an alias for LinkedList<type, true>.
	 *  @tparam  type  The type of values to store on this LinkedList.
	 */
	template <typename type>
	using DoublyLinkedList = LinkedList<type, true>;
};

#endif