#ifndef FLOW_BUFFER_ARRAY_HEADER
#define FLOW_BUFFER_ARRAY_HEADER

#include <bits/stdc++.h>

#include "../iterators/buffer-iterator.hpp"

namespace flow {
	namespace BufferErrors {
		enum BufferErrors {
			INDEX_OUT_OF_RANGE
		};
	};

	/**
	 *  @brief  An abstract class that represents a contiguous source of data
	 */
	template <typename type>
	class Buffer {
		public:
			// Abstract methods

			/**
			 *  @brief  Returns a pointer to the first element of the buffer.
			 */
			virtual type *data() const = 0;

			/**
			 *  @brief  Returns a read-only reference to the first element of
			 *  the buffer.
			 */
			virtual const type& front() const = 0;

			/**
			 *  @brief  Returns a read/write reference to the first element of
			 *  the buffer.
			 */
			virtual type& front() = 0;

			/**
			 *  @brief  Returns a read-only reference to the last element of
			 *  the buffer.
			 */
			virtual const type& back() const = 0;

			/**
			 *  @brief  Returns a read/write reference to the last element of
			 *  the buffer.
			 */
			virtual type& back() = 0;

			/**
			 *  @brief  Returns an Iterator to 
			 */

			/**
			 *  @brief  Returns the number of elements on the buffer.
			 */
			virtual size_t size() const = 0;

			/**
			 *  @brief  Gets the i-th element on the buffer.
			 *  @param  index  i
			 */
			virtual type get_at_index(size_t index) const = 0;

			/**
			 *  @brief  Sets the i-th element on the buffer.
			 *  @param  index  i
			 *  @param  value  The value to assign at the index.
			 */
			virtual void set_at_index(size_t index, const type& value) = 0;

			// Iterators

			/**
			 *  @brief  Read/write iterator for the data on the internal buffer.
			 *  Iteration is done in-order.
			 */
			using Iterator = BufferIterator<type, false>;

			/**
			 *  @brief  Read-only iterator for the data on the internal buffer.
			 *  Iteration is done in-order.
			 */
			using ConstIterator = BufferIterator<type, true>;

			/**
			 *  @brief  Returns a read/write iterator that points to the first
			 *  element of the Buffer. Iteration is done in-order.
			 */
			Iterator begin()
			{
				return Iterator(data());
			}

			/**
			 *  @brief  Returns a read/write iterator that points to one past
			 *  the last element of the Buffer. Iteration is done in-order.
			 */
			Iterator end()
			{
				return Iterator(data() + size());
			}

			/**
			 *  @brief  Returns a reverse read/write iterator that points to the
			 *  last element of the Buffer. Iteration is done in-order.
			 */
			Iterator rbegin()
			{
				return Iterator(data() + size() - 1);
			}

			/**
			 *  @brief  Returns a reverse read/write iterator that points to one
			 *  before the first element of the Buffer.
			 *  Iteration is done in-order.
			 */
			Iterator rend()
			{
				return Iterator(data() - 1);
			}

			/**
			 *  @brief  Returns a read-only iterator that points to the first
			 *  element of the Buffer. Iteration is done in-order.
			 */
			ConstIterator cbegin() const
			{
				return ConstIterator(data());
			}

			/**
			 *  @brief  Returns a read-only iterator that points to one past
			 *  the last element of the Buffer. Iteration is done in-order.
			 */
			ConstIterator cend() const
			{
				return ConstIterator(data() + size());
			}

			/**
			 *  @brief  Returns a reverse read-only iterator that points to the
			 *  last element of the Buffer. Iteration is done in-order.
			 */
			ConstIterator crbegin() const
			{
				return ConstIterator(data() + size() - 1);
			}

			/**
			 *  @brief  Returns a reverse read-only iterator that points to one
			 *  before the first element of the Buffer.
			 *  Iteration is done in-order.
			 */
			ConstIterator crend() const
			{
				return ConstIterator(data() - 1);
			}

			// Basic methods

			/**
			 *  @brief  Copies the values of other_buffer to this buffer.
			 *  @param  other_buffer  A reference to the buffer to copy from.
			 */
			void copy_from(const Buffer<type>& other_buffer)
			{
				size_t copy_size = sizeof(type) * other_buffer.size();
				memcpy(data(), other_buffer.data(), copy_size);
			}

			/**
			 *  @brief  Fills the buffer with a given value.
			 *  @param  value  The value to fill the buffer with.
			 */
			void fill(type value)
			{
				for (size_t i = 0; i < size(); i++) {
					set_at_index(i, value);
				}
			}

			/**
			 *  @brief  Swaps the elements at two given indices of the buffer.
			 *  @param  index_1  The first index
			 *  @param  index_2  The second index
			 */
			void swap_indices(size_t index_1, size_t index_2)
			{
				#ifdef DYNAMIC_ARRAY_SAFE_BOUNDS
				if (index_1 > size() || index_2 > size()) {
					throw BufferErrors::INDEX_OUT_OF_RANGE;
				}
				#endif

				type value_1 = get_at_index(index_1);
				*(data() + index_1) = *(data() + index_2);
				*(data() + index_2) = value_1;
			}

			/**
			 *  @brief  Sorts the buffer.
			 *  @param  compare_callback  The comparison function used to sort,
			 *  should return true if the first value is smaller than the second value.
			 */
			void sort(std::function<bool (type, type)> compare_callback)
			{
				std::sort(data(), data() + size(), compare_callback);
			}

			/**
			 *  @brief  Checks if all elements of two buffers are equal.
			 *  @param  other_buffer  A reference to the buffer to test equality.
			 */
			bool equals(Buffer<type>& other_buffer) const
			{
				// Check if the Buffers share memory address

				if (this == &other_buffer) return true;

				// Deep equality checking

				for (size_t i = 0; i < size(); i++) {
					if (get_at_index(i) != other_buffer.get_at_index(i)) return false;
				}

				return true;
			}

			/**
			 *  @brief  Checks if all elements of two buffers are equal.
			 *  @param  other_buffer  A reference to the buffer to test equality.
			 */
			bool operator==(Buffer<type>& other_buffer) const
			{
				return equals(other_buffer);
			}

			/**
			 *  @brief  Checks if not all elements of two buffers are equal.
			 *  @param  other_buffer  A reference to the buffer to test equality.
			 */
			bool operator!=(Buffer<type>& other_buffer) const
			{
				return !equals(other_buffer);
			}

			/**
			 *  @brief  Checks if the buffer has a certain value.
			 *  @param  value  The value to find.
			 */
			bool includes(type value) const
			{
				for (size_t i = 0; i < size(); i++) {
					if (get_at_index(i) == value) return true;
				}

				return false;
			}

			/**
			 *  @brief  Counts the number of times the buffer has a certain value.
			 *  @param  value  The value to count the number of appearances of.
			 */
			size_t count(type value) const
			{
				size_t found_values = 0;

				for (size_t i = 0; i < size(); i++) {
					if (get_at_index(i) == value) found_values++;
				}

				return found_values;
			}

			/**
			 *  @brief  Returns the index of the first occurrence of a given value.
			 *  If the value was not found, returns -1.
			 *  @param  value  The value to find the first index of.
			 *  @param  starting_index  The index to start searching at.
			 *  Defaults to 0.
			 */
			ssize_t first_index_of(type value, size_t starting_index = 0) const
			{
				for (int i = starting_index; i < size(); i++) {
					if (get_at_index(i) == value) return i;
				}

				return -1;
			}

			/**
			 *  @brief  Returns the index of last first occurrence of a given value.
			 *  If the value was not found, returns -1.
			 *  @param  value  The value to find the last index of.
			 *  @param  starting_index  The index to start searching at.
			 *  Defaults to size() - 1. The algorithm runs in decrementing order.
			 */
			ssize_t last_index_of(type value, size_t starting_index = size() - 1) const
			{
				for (int i = starting_index; i > 0; i--) {
					if (get_at_index(i) == value) return i;
				}

				return -1;
			}

			/**
			 *  @brief  Reverses the order of elements on a buffer.
			 */
			void reverse()
			{
				size_t left_index = 0;
				size_t right_index = size() - 1;

				while (right_index > left_index) {
					swap_indices(left_index++, right_index--);
				}
			}
	};
};


#endif