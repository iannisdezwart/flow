#ifndef FLOW_DYNAMIC_ARRAY_HEADER
#define FLOW_DYNAMIC_ARRAY_HEADER

#include <bits/stdc++.h>

using namespace std;

namespace flow {
	namespace DynamicArrayErrors {
		enum DynamicArrayErrors {
			INDEX_OUT_OF_RANGE
		};
	}

	/**
	 *  @brief  A resizable data structure that holds contigious elements.
	 *  @tparam  type  The type of data stored in the DynamicArray.
	 */
	template <typename type>
	class DynamicArray : public Buffer<type> {
		private:
			type *buffer = NULL;

			size_t current_element_count;
			size_t current_buffer_size;

			type *allocate_buffer()
			{
				return new type[current_buffer_size];
			}

			void reassign(
				initializer_list<type> values,
				size_t minimum_starting_size = 0
			)
			{
				size_t nearest_power_of_2 = pow(2, ceil(log2(current_element_count)));
				resize_buffer(max({ 16UL, nearest_power_of_2, minimum_starting_size }));

				current_element_count = values.size();

				// Copy the values into the DynamicArray

				for (size_t i = 0; i < current_element_count; i++) {
					buffer[i] = *(values.begin() + i);
				}
			}

			void resize_buffer(size_t new_buffer_size)
			{
				current_buffer_size = new_buffer_size;
				type *new_buffer = allocate_buffer();

				if (buffer != NULL) {
					// Copy the old values back onto the new buffer

					for (size_t i = 0; i < current_element_count; i++) {
						new_buffer[i] = buffer[i];
					}

					delete[] buffer;
				}

				buffer = new_buffer;
			}

			void shrink()
			{
				resize_buffer(current_buffer_size / 2);
			}

			void grow()
			{
				resize_buffer(current_buffer_size * 2);
			}

		public:
			/**
			 *  @brief  Creates a dynamic resizable array.
			 *  @param  starting_size  The number of elements to initially allocate.
			 */
			DynamicArray(size_t starting_size = 16)
			{
				current_element_count = 0;
				current_buffer_size = starting_size;
				buffer = allocate_buffer();
			}

			/**
			 *  @brief  Creates a dynamic resizable array.
			 *  @param  initial_values  The elements to initially put on the DynamicArray.
			 *  @param  minimum_starting_size  The minimum number of elements to
			 * initially allocate. Can be left empty for automatic size assignment.
			 */
			DynamicArray(
				initializer_list<type> initial_values,
				size_t minimum_starting_size = 0
			)
			{
				current_element_count = 0;
				reassign(initial_values, minimum_starting_size);
			}

			/**
			 *  @brief  Reassigns the array with new values.
			 *  @param  new_values  A brace enclosed list containing the new values.
			 */
			void operator=(initializer_list<type> new_values)
			{
				reassign(new_values);
			}

			/**
			 *  @brief  Releases the internal buffer and resets the DynamicArray.
			 *  Creates a new buffer with a given starting_size.
			 *  @param  starting_size  The initial size of the new buffer.
			 */
			void reset(size_t starting_size = 16)
			{
				type *new_buffer = allocate_buffer();
				if (buffer != NULL) delete[] buffer;

				current_element_count = 0;
				current_buffer_size = starting_size;
				buffer = new_buffer;
			}

			/**
			 *  @brief  Deletes the internal buffer.
			 */
			~DynamicArray()
			{
				if (buffer != NULL) delete[] buffer;
			}

			// Abstract method implementations

			/**
			 *  @brief  Returns a pointer to the first element of the DynamicArray.
			 */
			type *begin()
			{
				return buffer;
			}

			/**
			 *  @brief  Returns a pointer to the one after last element of the DynamicArray.
			 */
			type *end()
			{
				return buffer + current_element_count;
			}

			/**
			 *  @brief  Returns the current number of elements on the DynamicArray.
			 */
			size_t size()
			{
				return current_element_count;
			}

			/**
			 *  @brief  Returns the current internal buffer size of the DynamicArray.
			 */
			size_t current_capacity()
			{
				return current_buffer_size;
			}

			/**
			 *  @brief  Gets the i-th element on the buffer.
			 *  @param  index  i
			 */
			type get_at_index(size_t index)
			{
				if (index >= current_element_count) throw DynamicArrayErrors::INDEX_OUT_OF_RANGE;
				return buffer[index];
			}

			/**
			 *  @brief  Sets the i-th element on the buffer.
			 *  @param  index  i
			 *  @param  value  The value to assign at the index.
			 */
			void set_at_index(size_t index, type value)
			{
				if (index >= current_element_count) throw DynamicArrayErrors::INDEX_OUT_OF_RANGE;
				buffer[index] = value;
			}

			/**
			 *  @brief  Gets the i-th element on the buffer.
			 *  @param  index  i
			 */
			type & operator[](size_t index)
			{
				if (index >= current_element_count) throw DynamicArrayErrors::INDEX_OUT_OF_RANGE;
				return buffer[index];
			}

			/**
			 *  @brief  Sets the i-th element on the buffer.
			 *  @param  index  i
			 *  @param  =  The value to assign at the index.
			 */
			void operator[](size_t index) const
			{
				if (index >= current_element_count) throw DynamicArrayErrors::INDEX_OUT_OF_RANGE;
				return buffer[index];
			}

			/**
			 *  @brief  Returns the indices all occurrences of a given value.
			 *  @param  value  The value to find the indices of.
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(n), n = found indices
			 */
			DynamicArray<size_t> indices_of(type value)
			{
				DynamicArray<size_t> indices;

				for (int i = 0; i < size(); i++) {
					if (get_at_index(i) == value) indices.append(i);
				}

				return indices;
			}

			/**
			 *  @brief  Reserves space for some elements.
			 *  The DynamicArray will automatically grow to a power of 2 if needed.
			 * 	@param  size  The number of elements to reserve space for.
			 *  @note  Runtime: O(1) if no resize is needed, O(n) otherwise, n = size()
			 *  @note  Memory: O(1)
			 */
			void reserve(size_t size)
			{
				size_t new_size = current_element_count + size;

				if (new_size > current_buffer_size) {
					size_t growth_ratio = ceil((float) new_size / (float) current_buffer_size);

					// The number of times we must multiply by 2 to reserve space

					size_t growth_exponent = ceil(log2(growth_ratio));
					size_t new_buffer_size = current_buffer_size * pow(2, growth_exponent);

					// Resize the buffer

					resize_buffer(new_buffer_size);
				}
			}

			/**
			 *  @brief  Unreserves space for some elements.
			 *  The DynamicArray will automatically shrink by a factor of 2 if possible,
			 *  @param  size  The number of elements to unreserve spacce for.
			 *  @note  Runtime: O(1) if no resize is possible, O(n) otherwise, n = size()
			 *  @note  Memory: O(1)
			 */
			void unreserve(size_t size)
			{
				size_t new_size = current_element_count - size;
				if (new_size * 2 <= current_buffer_size) shrink();
			}

			/**
			 *  @brief  Creates a new DynamicArray by concatenating the values of
			 *  two existing DynamicArrays of the same data type.
			 *  @param  other_dynamic_array  The second DynamicArray to concatenate to
			 *  the first.
			 *  @note  Runtime: O(n + m), n = size(), m = other_dynamic_array.size()
			 *  @note  Memory: O(n + m), n = size(), m = other_dynamic_array.size()
			 */
			DynamicArray<type> operator+(DynamicArray<type>& other_dynamic_array)
			{
				size_t total_size = size() + other_dynamic_array.size();
				DynamicArray<type> concatenated_dynamic_array(total_size);

				concatenated_dynamic_array.attach(*this);
				concatenated_dynamic_array.attach(other_dynamic_array);

				return concatenated_dynamic_array;
			}

			/**
			 *  @brief  Directly increments the private property current_element_count
			 *  by a certain amount. ONLY USE this if you know what you are doing!!!
			 *  @param  number_of_elements  By how much you want to increment
			 *  current_element_count.
			 */
			void unsafe_increment_element_count(size_t number_of_elements)
			{
				current_element_count += number_of_elements;
			}

			/**
			 *  @brief  Directly decrements the private property current_element_count
			 *  by a certain amount. ONLY USE this if you know what you are doing!!!
			 *  @param  number_of_elements  By how much you want to decrement
			 *  current_element_count.
			 */
			void unsafe_decrement_element_count(size_t number_of_elements)
			{
				current_element_count -= number_of_elements;
			}

			/**
			 *  @brief  WARNING: Does not check for buffer overflows AND does not
			 *  increment the number of elements!!! ALWAYS USE
			 *  DynamicArray::unsafe_increment_element_count() after calling this
			 *  method!!!
			 *  Appends a new element to the back of the DynamicArray.
			 *  @param  value  The value to push to the DynamicArray.
			 *  @param  offset  The offset after the current last element to place
			 *  the new value.
			 */
			void unsafe_append(type value, size_t offset = 0)
			{
				buffer[current_element_count + offset] = value;
			}

			/**
			 *  @brief  Appends a new element to the back of the DynamicArray.
			 *  The DynamicArray will automatically grow by a factor of 2 if needed.
			 *  @param  value  The value to push to the DynamicArray.
			 */
			void append(type value)
			{
				// Grow array if needed

				if (current_element_count >= current_buffer_size) grow();
				buffer[current_element_count++] = value;
			}

			/**
			 *  @brief  Appends a new element to the back of the DynamicArray.
			 *  The DynamicArray will automatically grow by a factor of 2 if needed.
			 *  @param  value  The value to push to the DynamicArray.
			 */
			void operator+=(type value)
			{
				append(value);
			}

			/**
			 *  @brief  Returns the last element of the DynamicArray and deletes it.
			 *  The DynamicArray will automatically shrink by a factor of 2 when possible.
			 *  @note  Runtime: O(1) if no resize is needed, O(n) otherwise, n = size()
			 *  @note  Memory: O(1)
			 */
			type extract_rear()
			{
				type value = buffer[current_element_count--];

				// Shrink array if possible

				if (current_element_count <= current_buffer_size / 2) shrink();
				return value;
			}

			/**
			 *  @brief  Places a new element to the the beginning of the DynamicArray.
			 *  The other elements will be shifted one place to the right.
			 *  The DynamicArray will automatically grow by a factor of 2 if needed.
			 *  @param  value  The value to place in front of the DynamicArray.
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(1)
			 */
			void prepend(type value)
			{
				// Grow array if needed

				if (current_element_count >= current_buffer_size) grow();
				current_element_count++;

				// Shift elements one spot to the right

				size_t i = current_element_count;

				while (true) {
					buffer[i + 1] = buffer[i];
					if (i == 0) break;
					i--;
				}

				buffer[0] = value;
			}

			/**
			 *  @brief  Returns the first element of the DynamicArray and deletes it.
			 *  The other elements will be shifted one place to the left.
			 *  The DynamicArray will automatically shrink by a factor of 2 when possible.
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(1)
			 */
			type extract_front()
			{
				type value = buffer[0];

				for (size_t i = 0; i < current_element_count - 1; i++) {
					buffer[i] = buffer[i + 1];
				}

				current_element_count--;

				// Shrink array if possible

				if (current_element_count <= current_buffer_size / 2) shrink();
				return value;
			}

			/**
			 *  @brief  Concatenates an other DynamicArray at the end of this DynamicArray.
			 *  The DynamicArray will automatically grow to a power of 2 if needed.
			 *  @param other_dynamic_array  The DynamicArray to attach to this DynamicArray.
			 *  @note  Runtime: O(m), m = other_dynamic_array.size(). If a resize is needed,
			 *  O(n + m), n = size()
			 *  @note  Memory: O(1)
			 */
			void attach(DynamicArray<type>& other_dynamic_array)
			{
				// Reserve space

				reserve(other_dynamic_array.size());

				// Copy all elements from other_dynamic_array to this DynamicArray

				for (size_t i = 0; i < other_dynamic_array.size(); i++) {
					// Don't worry, we reserved enough space and are calling
					// unsafe_increment_element_count afterwards

					unsafe_append(other_dynamic_array.get_at_index(i), i);
				}

				unsafe_increment_element_count(other_dynamic_array.size());
			}

			/**
			 *  @brief  Concatenates an other DynamicArray at the end of this DynamicArray.
			 *  The DynamicArray will automatically grow to a power of 2 if needed.
			 *  @param other_dynamic_array  The DynamicArray to attach to this DynamicArray.
			 *  @note  Runtime: O(m), m = other_dynamic_array.size(). If a resize is needed,
			 *  O(n + m), n = size()
			 *  @note  Memory: O(1)
			 */
			void operator+=(DynamicArray<type>& other_dynamic_array)
			{
				attach(other_dynamic_array);
			}

			/**
			 *  @brief  Shifts the elements on this DynamicArray to the right
			 *  by some offset. The items on the left will stay as they were.
			 *  WATCH OUT for undefined bahaviour!
			 *  Use DynamicArray::shift_left() to shift elements to the left.
			 *  WARNING: elements will fall off if there is no space.
			 *  Use DynamicArray::rotate() to circularly rotate the elements.
			 *  @param  offset  How many positions to shift the message to the right.
			 *  @param  starting_index  The first index to shift, counted from the left.
			 *  The items to the left of this index will not be shifted. Defaults to 0.
			 *  @param  length  The max amount of values to shift. Defaults to SIZE_MAX.
			 *  @note  Runtime: O(l), l = size() or length
			 *  @note  Memory: O(1)
			 */
			void shift_right(
				size_t offset,
				size_t starting_index = 0,
				size_t length = SIZE_MAX
			)
			{
				size_t i = min(current_buffer_size, current_buffer_size - offset);
				i = min(i, starting_index + length);

				while (i > starting_index) {
					buffer[i - 1 + offset] = buffer[i - 1];
					i--;
				}
			}

			/**
			 *  @brief  Shifts the elements on this DynamicArray to the left
			 *  by some offset. The items on the right will stay as they were.
			 *  WATCH OUT for undefined bahaviour!
			 *  Use DynamicArray::shift_right() to shift elements to the left.
			 *  WARNING: elements will fall off if there is no space.
			 *  Use DynamicArray::rotate() to circularly rotate the elements.
			 *  @param  offset  How many positions to shift the message to the left.
			 *  @param  starting_index  The first index to shift, counted from the left.
			 *  The items to the left of this index will not be shifted. Defaults to 0.
			 *  @param  length  The max amount of values to shift. Defaults to SIZE_MAX.
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(1)
			 */
			void shift_left(
				size_t offset,
				size_t starting_index = 0,
				size_t length = SIZE_MAX
			)
			{
				size_t i = starting_index - offset;
				size_t upper_bound = min(size() - offset, starting_index + length);

				while (i < upper_bound) {
					buffer[i] = buffer[i + offset];
					i++;
				}
			}

			/**
			 *  @brief  Shifts the elements on this DynamicArray to the right
			 *  by some offset. The items that would fall off on the right, are
			 *  circularly prepended to the left. Use a negative sign to rotate left.
			 *  Use DynamicArray::shift_right() or DynamicArray::shift_right() to
			 *  do the same but delete the items that would fall off.
			 *  @param  offset  The offset by which to rotate the array
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(1)
			 */
			void rotate(ssize_t offset)
			{
				offset *= -1;
				offset %= (ssize_t) size();
				if (offset < 0) offset += size();

				size_t off = offset;
				size_t divisor = __gcd(off, size());

				for (size_t i = 0; i < divisor; i++) {
					// Move i-th values of blocks

					type stored = get_at_index(i);
					size_t j = i;

					while (true) {
						size_t next = j + off;

						if (next >= size()) next -= size();
						if (next == i) break;

						set_at_index(j, get_at_index(next));
						j = next;
					}

					set_at_index(j, stored);
				}
			}
	};
};

#endif
