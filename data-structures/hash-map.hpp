#ifndef FLOW_HASH_MAP_HEADER
#define FLOW_HASH_MAP_HEADER

#include <bits/stdc++.h>

#include "dynamic-array.hpp"
#include "string.hpp"
#include "linked-list.hpp"

namespace flow_hash_map_tools {
	using namespace flow;

	template <typename Key, typename Value>
	class KeyValuePair {
		public:
			Key key;
			Value value;

			KeyValuePair() {}
			KeyValuePair(const Key& key, const Value& value) : key(key), value(value) {}
	};

	enum class HashMapErrors {
		KEY_NOT_FOUND
	};

	template <typename Key, typename Value>
	class HashMapTable {
		public:
			typedef KeyValuePair<Key, Value> Entry;

			size_t cur_size = 0;
			DynamicArray<LinkedList<Entry>> table;

			HashMapTable(size_t table_size) : table(table_size)
			{
				table.unsafe_set_element_count(table.current_capacity());
			}

			HashMapTable(const HashMapTable<Key, Value>& other)
			{
				cur_size = other.cur_size;
				table = other.table;
			}

			HashMapTable(HashMapTable<Key, Value>&& other)
			{
				cur_size = other.cur_size;
				table = std::move(other.table);

				other.cur_size = 0;
			}

			HashMapTable<Key, Value>& operator=(const HashMapTable<Key, Value>& other)
			{
				if (this == &other) return *this;

				cur_size = other.cur_size;
				table = other.table;

				return *this;
			}

			HashMapTable<Key, Value>& operator=(HashMapTable<Key, Value>&& other)
			{
				if (this == &other) return *this;

				cur_size = other.cur_size;
				table = std::move(other.table);

				other.cur_size = 0;

				return *this;
			}

			template <bool Const = false>
			class IteratorBase {
				private:
					DynamicArray<LinkedList<Entry>>& table;
					size_t list_index;
					typename LinkedList<Entry>::Iterator list_it;

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
						DynamicArray<LinkedList<Entry>>& table,
						size_t list_index
					) : table(table), list_index(list_index), list_it(NULL)
					{
						if (list_index == table.size()) return;
						hook_to_next_node();
					}

					IteratorBase(const IteratorBase& other)
						: table(other.table), list_index(other.list_index),
							list_it(other.list_it) {}

					IteratorBase<Const>& operator=(const IteratorBase<Const>& other)
					{
						if (this == &other) return *this;

						list_index = other.list_index;
						list_it = other.list_it;
						table = other.table;

						return *this;
					}

					size_t get_list_index()
					{
						return list_index;
					}

					typename LinkedList<Entry>::Iterator& get_list_it()
					{
						return list_it;
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

					IteratorBase<Const>& /* prefix */ operator++()
					{
						list_it++;

						if (list_it.get_node_pointer()->next == NULL) {
							list_index++;
							hook_to_next_node();
						}

						return *this;
					}

					IteratorBase<Const> /* postfix */ operator++(int)
					{
						IteratorBase<Const> old_it = *this;
						list_it++;

						if (list_it.get_node_pointer()->next == NULL) {
							list_index++;
							hook_to_next_node();
						}

						return old_it;
					}

					bool operator==(const IteratorBase<Const>& other)
					{
						return list_index == other.list_index && list_it == other.list_it;
					}

					bool operator!=(const IteratorBase<Const>& other)
					{
						return list_index != other.list_index || list_it != other.list_it;
					}
			};

			using Iterator = IteratorBase<false>;
			using ConstIterator = IteratorBase<true>;

			Iterator begin()
			{
				return Iterator(table, 0);
			}

			Iterator end()
			{
				return Iterator(table, table.size());
			}

			ConstIterator cbegin()
			{
				return ConstIterator(table, 0);
			}

			ConstIterator cend()
			{
				return ConstIterator(table, table.size());
			}

			LinkedList<Entry>& get_list_of_key(const Key& key)
			{
				struct std::hash<Key> hash_func;
				size_t index = hash_func(key) & table.size() - 1;
				return table[index];
			}

			Value& get_by_key(const Key& key)
			{
				LinkedList<Entry>& list = get_list_of_key();

				for (const Entry& entry : list) {
					if (entry.key == key) return entry.value;
				}

				throw HashMapErrors::KEY_NOT_FOUND;
			}

			double avg_list_size()
			{
				return (double) cur_size / (double) table.size();
			}

			bool insert(const Key& key, const Value& value)
			{
				LinkedList<Entry>& list = get_list_of_key(key);

				for (Entry& entry : list) {
					if (entry.key == key) {
						entry.value = value;
						return false;
					}
				}

				cur_size++;
				list.append(Entry(key, value));
				return false;
			}

			bool remove(const Key& key)
			{
				LinkedList<Entry>& list = get_list_of_key(key);

				typename LinkedList<Entry>::Iterator it = list.begin();
				typename LinkedList<Entry>::Iterator end = list.end();

				while (it != end) {
					if (it->key == key) {
						list.remove(it);
						cur_size--;
						return true;
					}
				}

				return false;
			}

			void print()
			{
				for (size_t i = 0; i < table.size(); i++) {
					LinkedList<Entry>& list = table[i];

					String::format("=== Bucket %llu (%llu) ===",
						i, list.size()).print();

					for (const Entry& entry : list) {
						String::format("%lld -> %lld", entry.key, entry.value).print();
					}
				}
			}
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
			constexpr static const size_t MIN_TABLE_SIZE = 16;
			constexpr static const double MIN_ALPHA = 0.3;
			constexpr static const double MAX_ALPHA = 1.0;

		protected:
			HashMapTable<Key, Value> table;

		private:
			LinkedList<Entry *>& get_list_of_key(const Key& key)
			{
				return table.get_by_key(key);
			}

			double avg_list_size()
			{
				return table.avg_list_size();
			}

			void grow()
			{
				HashMapTable<Key, Value> new_table(table.table.size() << 1);

				for (const Entry& entry : table) {
					new_table.insert(entry.key, entry.value);
				}

				table = std::move(new_table);
			}

			void shrink()
			{
				HashMapTable<Key, Value> new_table(table.table.size() >> 1);

				for (const Entry& entry : table) {
					new_table.insert(entry.key, entry.value);
				}

				table = std::move(new_table);
			}

		public:
			/**
			 *  @brief  Creates a HashMap of a given initial table size.
			 *  @param  init_table_size  The initial table size.
			 */
			HashMap(size_t init_table_size = 16) : table(init_table_size) {}

			/**
			 *  @brief  Creates a copy of another HashMap.
			 *  @param  other  The HashMap to copy.
			 */
			HashMap(const HashMap<Key, Value>& other) : table(other.table) {}

			/**
			 *  @brief  Creates a HashMap by moving from an rvalue HashMap.
			 *  @param  other  The HashMap to move.
			 */
			HashMap(HashMap<Key, Value>&& other) : table(std::move(other.table)) {}

			/**
			 *  @brief  Copies the entries of another HashMap into this HashMap.
			 *  All existing entries are removed.
			 */
			HashMap<Key, Value>& operator=(const HashMap<Key, Value>& other)
			{
				if (this == &other) return *this;

				table = other.table;
				return *this;
			}

			/**
			 *  @brief  Moves the entries of another HashMap into this HashMap.
			 *  All existing entries are removed.
			 */
			HashMap<Key, Value>& operator=(HashMap<Key, Value>&& other)
			{
				if (this == &other) return *this;

				table = std::move(other.table);
				return *this;
			}

			/**
			 *  @brief  Returns the current number of entries on the HashMap.
			 */
			size_t size() const
			{
				return table.cur_size;
			}

			/**
			 *  @brief  Read/write iterator for the entries in the HashMap.
			 *  Iteration order is undefined.
			 */
			using Iterator = typename HashMapTable<Key, Value>::Iterator;

			/**
			 *  @brief  Read-only iterator for the entries in the HashMap.
			 *  Iteration order is undefined.
			 */
			using ConstIterator = typename HashMapTable<Key, Value>::ConstIterator;

			/**
			 *  @brief  Returns a read/write iterator that points to the first
			 *  entry of the HashMap. Iteration order is undefined.
			 */
			Iterator begin()
			{
				return table.begin();
			}

			/**
			 *  @brief  Returns a read/write iterator that points to one past
			 *  the last entry of the HashMap. Iteration order is undefined.
			 */
			Iterator end()
			{
				return table.end();
			}

			/**
			 *  @brief  Returns a read-only iterator that points to the first
			 *  entry of the HashMap. Iteration order is undefined.
			 */
			ConstIterator cbegin()
			{
				return table.cbegin();
			}

			/**
			 *  @brief  Returns a read-only iterator that points to one past
			 *  the last entry of the HashMap. Iteration order is undefined.
			 */
			ConstIterator cend()
			{
				return table.cend();
			}

			/**
			 *  @brief  Returns a read-only reference to an entry in the HashMap.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			const Value& operator[](const Key& key) const
			{
				return table.get_by_key(key);
			}

			/**
			 *  @brief  Returns a read/write reference to an entry in the HashMap.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			Value& operator[](const Key& key)
			{
				return table.get_by_key(key);
			}

			/**
			 *  @brief  Checks if a key is present in the HashMap.
			 *  @note  Runtime: O(1)
			 *  @note  Memory: O(1)
			 */
			bool has_key(const Key& key) const
			{
				LinkedList<Entry>& list = get_list_of_key(key);

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
				if (avg_list_size() >= HashMap::MAX_ALPHA) grow();

				return table.insert(key, value);
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
				if (avg_list_size() <= HashMap::MIN_ALPHA
					&& (table.table.size() >> 1) > HashMap::MIN_TABLE_SIZE) shrink();

				return table.remove(key);
			}

			/**
			 *  @brief  Prints the current state of the HashMap.
			 */
			void print()
			{
				table.print();
			}
	};
};

#endif