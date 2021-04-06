#ifndef FLOW_HASH_MAP_HEADER
#define FLOW_HASH_MAP_HEADER

#include <bits/stdc++.h>

#include "dynamic-array.hpp"
#include "string.hpp"
#include "linked-list.hpp"

namespace flow_hash_map_tools {
	template <typename Key, typename Value>
	class KeyValuePair {
		public:
			Key key;
			Value value;

			KeyValuePair(const Key& key, const Value& value) : key(key), value(value) {}
	};

	enum class HashMapErrors {
		KEY_NOT_FOUND
	};
};

namespace flow {
	using namespace flow_hash_map_tools;

	/**
	 *  @brief  HashMap implementation. Lookups, insertion and updating
	 *  takes O(1) on average.
	 */
	template <typename Key, typename Value>
	class HashMap {
		public:
			typedef KeyValuePair<Key, Value> Entry;

		private:
			DoublyLinkedList<Entry>& get_list_of_key(const Key& key)
			{
				struct std::hash<Key> hash_func;
				size_t index = hash_func(key) % table.size();
				return table[index];
			}

			Value& get_by_key(const Key& key)
			{
				DoublyLinkedList<Entry>& list = get_list_of_key(key);

				for (const Entry& entry : list) {
					if (entry.key == key) {
						return entry.value;
					}
				}

				throw HashMapErrors::KEY_NOT_FOUND;
			}

			/*
				[ 1, 2, 3 ],    <--- (2)
				[ 4 ],          <--- (1)
				[ ],            <--- (0)
				[ 5, 6 ],
				[ ],
				[ 7 ]
			*/

			template <bool Const = false>
			class IteratorBase {
				private:
					DynamicArray<DoublyLinkedList<Entry>>& table;
					size_t list_index;
					typename DoublyLinkedList<Entry>::Iterator list_it;

					void hook_to_next_node()
					{
						while (true) {
							if (list_index == table.size()) {
								list_it = NULL;
								break;
							}

							if (table[list_index].size() > 0) {
								list_it = table[list_index].begin();
								break;
							}

							list_index++;
						}
					}

				public:
					IteratorBase(
						DynamicArray<DoublyLinkedList<Entry>>& table,
						size_t list_index
					) : table(table), list_index(list_index), list_it(NULL)
					{
						if (list_index == table.size()) return;
						hook_to_next_node();
					}

					size_t get_list_index()
					{
						return list_index;
					}

					typename DoublyLinkedList<Entry>::Iterator& get_list_it()
					{
						return list_it;
					}

					void operator=(const IteratorBase& other_iterator)
					{
						list_index = other_iterator.list_index;
						list_it = other_iterator.list_it;
					}

					const Entry& operator*() const
					{
						return *list_it;
					}

					template <bool T = true>
					typename std::enable_if<T && !Const, Entry&>::type
					/* Entry& */ operator*()
					{
						return *list_it;
					}

					IteratorBase& /* prefix */ operator++()
					{
						list_it++;

						if (list_it.get_node_pointer() == NULL) {
							list_index++;
							hook_to_next_node();
						}

						return *this;
					}

					IteratorBase /* postfix */ operator++(int)
					{
						IteratorBase old_it = *this;
						list_it++;

						if (list_it.get_node_pointer() == NULL) {
							list_index++;
							hook_to_next_node();
						}

						return old_it;
					}

					bool operator==(const IteratorBase& other)
					{
						return list_index == other.list_index && list_it == other.list_it;
					}

					bool operator!=(const IteratorBase& other)
					{
						return list_index != other.list_index || list_it != other.list_it;
					}
			};

		protected:
			size_t cur_size;
			DynamicArray<DoublyLinkedList<Entry>> table;

		public:
			HashMap(size_t init_table_size = 16)
				: cur_size(0), table(init_table_size)
			{
				table.unsafe_set_element_count(init_table_size);

				for (size_t i = 0; i < table.size(); i++) {
					table[i] = DoublyLinkedList<Entry>();
				}
			}

			/**
			 *  @brief  Read/write iterator for the data in the HashMap.
			 *  Iteration order is undefined.
			 */
			using Iterator = IteratorBase<false>;

			/**
			 *  @brief  Read-only iterator for the data in the HashMap.
			 *  Iteration order is undefined.
			 */
			using ConstIterator = IteratorBase<true>;

			/**
			 *  @brief  Returns a read/write iterator that points to the first
			 *  element of the HashMap. Iteration order is undefined.
			 */
			Iterator begin()
			{
				return Iterator(table, 0);
			}

			/**
			 *  @brief  Returns a read/write iterator that points to one past
			 *  the last element of the HashMap. Iteration order is undefined.
			 */
			Iterator end()
			{
				return Iterator(table, table.size());
			}

			/**
			 *  @brief  Returns a read-only reference to a value in the HashMap.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			const Value& operator[](const Key& key) const
			{
				return get_by_key(key);
			}

			/**
			 *  @brief  Returns a read/write reference to a value in the HashMap.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			Value& operator[](const Key& key)
			{
				return get_by_key(key);
			}

			/**
			 *  @brief  Checks if a key is present in the HashMap.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			bool has_key(const Key& key)
			{
				DoublyLinkedList<Entry>& list = get_list_of_key(key);

				for (const Entry& entry : list) {
					if (entry.key == key) return true;
				}

				return false;
			}

			/**
			 *  @brief  Inserts an entry into the HashMap. If the key exists,
			 *  the value is overwritten.
			 *  @returns  False if the key already existed and the corresponding
			 *  value was updated, true if a new entry was created.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			bool insert(const Key& key, const Value& value)
			{
				DoublyLinkedList<Entry>& list = get_list_of_key(key);

				for (Entry& entry : list) {
					if (entry.key == key) {
						entry.value = value;
						return false;
					}
				}

				list.append(KeyValuePair<Key, Value>(key, value));
				return true;
			}

			/**
			 *  @brief  Removes an entry from the HashMap by its key.
			 *  @returns  True if an entry was removed, false if no entry with
			 *  the given key was found.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			bool remove(const Key& key)
			{
				DoublyLinkedList<Entry>& list = get_list_of_key(key);
				typename DoublyLinkedList<Entry>::Iterator it = list.begin();
				typename DoublyLinkedList<Entry>::Iterator end = list.end();

				while (it != end) {
					if (it->key == key) {
						list.remove(it);
						return true;
					}
				}

				return false;
			}

			/**
			 *  @brief  Prints the current state of the HashMap.
			 */
			void print()
			{
				for (size_t i = 0; i < table.size(); i++) {
					DoublyLinkedList<Entry>& list = table[i];

					String::format("=== Bucket %llu (%llu) ===",
						i, list.size()).print();

					for (const Entry& entry : list) {
						String::format("%lld -> %lld", entry.key, entry.value).print();
					}
				}
			}
	};
};

#endif