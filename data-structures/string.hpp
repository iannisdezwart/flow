#ifndef FLOW_STRING_HEADER
#define FLOW_STRING_HEADER

#include <bits/stdc++.h>

#include "dynamic-array.hpp"
#include "string-tools.hpp"

namespace flow {
	/**
	 *  @brief  A flexible string of characters.
	 */
	class String : public DynamicArray<char> {
		public:
			/**
			 *  @brief  Creates a String with a given initial capacity.
			 *  @param  capacity  The initial capacity of the String.
			 */
			String(size_t capacity = 16) : DynamicArray<char>(capacity) {}

			/**
			 *  @brief  Creates a String from a sequence of characters.
			 *  @param  chars  A reference to the sequence of characters.
			 */
			template <size_t char_count>
			String(const char (&chars)[char_count]) : DynamicArray<char>(char_count - 1)
			{
				unsafe_set_element_count(char_count - 1);
				memcpy(buffer, chars, char_count - 1);
			}

			/**
			 *  @brief  Creates a String from a NULL terminated char pointer.
			 *  @param  chars  A pointer to the NULL terminated char pointer.
			 */
			String(const char *chars) : DynamicArray<char>(strlen(chars))
			{
				size_t len = strlen(chars);
				unsafe_set_element_count(len);
				memcpy(buffer, chars, len);
			}

			/**
			 *  @brief  Creates a copy of another String.
			 *  @param  other  The String to copy.
			 */
			String(const String& other) : DynamicArray<char>(other.current_capacity())
			{
				unsafe_set_element_count(other.size());
				memcpy(buffer, other.buffer, other.size());
			}

			/**
			 *  @brief  Creates a String by moving from an rvalue String.
			 *  @param  other  The String to move.
			 */
			String(String&& other) : DynamicArray<char>(other.current_capacity())
			{
				unsafe_set_element_count(other.size());
				buffer = other.buffer;

				other.buffer = NULL;
				other.current_element_count = 0;
			}

			/**
			 *  @brief  Deletes the current value of this String and copies a new
			 *  character array to this String.
			 *  @param  new_value  New character array to copy to this String.
			 */
			template <size_t char_count>
			String& operator=(const char (&chars)[char_count])
			{
				size_t new_size = char_count - 1;

				reset(new_size);
				unsafe_set_element_count(new_size);

				for (size_t i = 0; i < new_size; i++) {
					set_at_index(i, chars[i]);
				}

				return *this;
			}

			/**
			 *  @brief  Deletes the current value of this String and copies another
			 *  String to this String.
			 *  @param  other  New String to copy to this String.
			 */
			String& operator=(const String& other)
			{
				if (this == &other) return *this;

				size_t new_size = other.size();

				reset(other.current_capacity());
				unsafe_set_element_count(new_size);
				copy_from(other);

				return *this;
			}

			/**
			 *  @brief  Creates a String by moving from an rvalue String.
			 *  @param  other  The String to move.
			 */
			String& operator=(String&& other)
			{
				if (this == &other) return *this;

				delete[] buffer;
				current_buffer_size = other.current_buffer_size;
				current_element_count = other.current_element_count;
				buffer = other.buffer;

				other.buffer = NULL;
				other.current_element_count = 0;

				return *this;
			}

			/**
			 *  @brief  Checks if two Strings are equal.
			 */
			bool operator==(const String& other_str) const
			{
				if (size() != other_str.size()) return false;

				for (size_t i = 0; i < size(); i++) {
					if (get_at_index(i) != other_str[i]) return false;
				}

				return true;
			}

			/**
			 *  @brief  Checks if two Strings are not equal.
			 */
			bool operator!=(const String& other_str) const
			{
				return !operator==(other_str);
			}

			/**
			 *  @brief  Ensures there is a NULL byte after the String and returns a
			 *  pointer to the first character of the internal buffer of this String,
			 *  effectively resolving a constant character array.
			 *  @note  Runtime: O(1) if no resize is needed, otherwise O(n), n = size()
			 *  @note  Memory: O(1)
			 */
			const char *to_char_arr()
			{
				append('\0');
				unsafe_decrement_element_count(1);
				return data();
			}

			/**
			 *  @brief  Attaches a sequence of characters to the end of this String.
			 *  The String will automatically grow to a power of 2 if needed.
			 *  @param  chars  The sequence of characters to attach to this String.
			 *  @note  Runtime: O(n), n = size() + char_count
			 *  @note  Memory: O(1)
			 */
			template <size_t char_count>
			void attach(const char (&chars)[char_count])
			{
				// Allocate space for all new characters

				reserve(char_count - 1);

				// Attach the characters to this String

				for (size_t i = 0; i < char_count - 1; i++) {
					// Don't worry, we reserved enough space and are calling
					// unsafe_increment_element_count afterwards

					unsafe_append(chars[i], i);
				}

				unsafe_increment_element_count(char_count - 1);
			}

			/**
			 *  @brief  Attaches a null terminated string to the end of this String.
			 *  The String will automatically grow to a power of 2 if needed.
			 *  @param  str  The null terminated string to attach to this String.
			 *  @note  Runtime: O(n), n = size() + char_count
			 *  @note  Memory: O(1)
			 */
			void attach_c_str(const char *str)
			{
				// Allocate space for all new characters

				size_t len = strlen(str);
				reserve(len);

				// Attach the characters to this String

				for (size_t i = 0; i < len; i++) {
					// Don't worry, we reserved enough space and are calling
					// unsafe_increment_element_count afterwards

					unsafe_append(str[i], i);
				}

				unsafe_increment_element_count(len);
			}

			/**
			 *  @brief  Attaches another String to the end of this String.
			 *  The String will automatically grow to a power of 2 if needed.
			 *  @param  str  The other String to attach to this String.
			 *  @note  Runtime: O(n), n = size() + str.size()
			 *  @note  Memory: O(1)
			 */
			void attach(const String& str)
			{
				// Allocate space for all new characters

				reserve(str.size());

				// Attach the characters to this String

				for (size_t i = 0; i < str.size(); i++) {
					// Don't worry, we reserved enough space and are calling
					// unsafe_increment_element_count afterwards

					unsafe_append(str[i], i);
				}

				unsafe_increment_element_count(str.size());
			}

			/**
			 *  @brief  Attaches a sequence of characters to the end of this String.
			 *  The String will automatically grow to a power of 2 if needed.
			 *  @param  chars  The sequence of characters to attach to this String.
			 *  @note  Runtime: O(n), n = size() + char_count
			 *  @note  Memory: O(1)
			 */
			template <size_t char_count>
			void operator+=(const char (&chars)[char_count]) { attach(chars); }

			/**
			 *  @brief  Attaches another String to the end of this String.
			 *  The String will automatically grow to a power of 2 if needed.
			 *  @param  str  The other String to attach to this String.
			 *  @note  Runtime: O(n), n = size() + str.size()
			 *  @note  Memory: O(1)
			 */
			void operator+=(const String& str) { attach(str); }

			// Prevent C++ inherited class Name Hiding

			using DynamicArray::operator+=;

			/**
			 *  @brief  Alter this String by repeats it a certain amount of times.
			 *  @param  repeat_count  The number of times to repeat this String.
			 *  @note  Runtime: O(n * m), n = size() - 1, m = repeat_count
			 *  @note  Memory: O(1)
			 */
			void repeat(size_t repeat_count)
			{
				size_t new_string_size = size() * repeat_count;
				reserve(new_string_size - size());

				// Append the String repeat_count - 1 times

				for (size_t i = 0; i < repeat_count - 1; i++) {
					for (size_t j = 0; j < size(); j++) {
						// Don't worry, we reserved enough space and are calling
						// unsafe_increment_element_count afterwards

						unsafe_append(get_at_index(j), i * size() + j);
					}
				}

				unsafe_increment_element_count(new_string_size - size());
			}

			/**
			 *  @brief  Alter this String by repeats it a certain amount of times.
			 *  @param  repeat_count  The number of times to repeat this String.
			 *  @note  Runtime: O(n * m), n = size() - 1, m = repeat_count
			 *  @note  Memory: O(1)
			 */
			void operator*=(size_t repeat_count)
			{
				repeat(repeat_count);
			}

			/**
			 *  @brief  Concatenates two Strings into one new String and returns it.
			 *  @param  other_string  The second String that is appended to the first.
			 *  @note  Runtime: O(n + m), n = size(), m = other_string.size()
			 *  @note  Memory: O(n + m)
			 */
			String concatenate(String& other_string) const
			{
				size_t new_string_size = size() + other_string.size();
				String new_string(new_string_size);

				// Push the first String

				for (size_t i = 0; i < size(); i++) {
					new_string.append(get_at_index(i));
				}

				// Push the second String

				for (size_t i = 0; i < other_string.size(); i++) {
					new_string.append(other_string.get_at_index(i));
				}

				return new_string;
			}

			/**
			 *  @brief  Concatenates two Strings into one new String and returns it.
			 *  @param  other_string  The second String that is appended to the first.
			 *  @note  Runtime: O(n + m), n = size(), m = other_string.size()
			 *  @note  Memory: O(n + m)
			 */
			String operator+(String& other_string) const
			{
				return concatenate(other_string);
			}

			/**
			 *  @brief  Concatenates this String and a sequence of characters into
			 *  one new String and returns it.
			 *  @param  chars  The characters that are appended to this String.
			 *  @note  Runtime: O(n + m), n = size(), m = other_string.size()
			 *  @note  Memory: O(n + m)
			 */
			template <size_t char_count>
			String concatenate(const char (&chars)[char_count]) const
			{
				size_t new_string_size = size() + char_count - 1;
				String new_string(new_string_size);

				// Push the first String

				for (size_t i = 0; i < size(); i++) {
					new_string.append(get_at_index(i));
				}

				// Push the second String

				for (size_t i = 0; i < char_count - 1; i++) {
					new_string.append(chars[i]);
				}

				return new_string;
			}

			/**
			 *  @brief  Concatenates this String and a sequence of characters into
			 *  one new String and returns it.
			 *  @param  chars  The characters that are appended to this String.
			 *  @note  Runtime: O(n + m), n = size(), m = other_string.size()
			 *  @note  Memory: O(n + m)
			 */
			template <size_t char_count>
			String operator+(const char (&chars)[char_count]) const
			{
				return concatenate(chars);
			}

			/**
			 *  @brief  Creates a new String by repeating this String a certain
			 *  amount of times.
			 *  @param  repeat_count  The number of times to repeat this String.
			 *  @note  Runtime: O(n * m), n = size(), m = repeat_count
			 *  @note  Memory: O(n * m)
			 */
			String duplicate(size_t repeat_count) const
			{
				size_t new_string_size = size() * repeat_count;
				String new_string(new_string_size);

				// Push the String repeat_count times

				for (size_t i = 0; i < repeat_count; i++) {
					for (size_t j = 0; j < size(); j++) {
						new_string.append(get_at_index(j));
					}
				}

				return new_string;
			}

			/**
			 *  @brief  Creates a new String by repeating this String a certain
			 *  amount of times.
			 *  @param  repeat_count  The number of times to repeat this String.
			 *  @note  Runtime: O(n * m), n = size(), m = repeat_count
			 *  @note  Memory: O(n * m)
			 */
			String operator*(size_t repeat_count) const
			{
				return duplicate(repeat_count);
			}

			/**
			 *  @brief  Checks whether this String ends with a certain character sequence.
			 *  @param  chars  The characters to compare the end of this String with.
			 *  @note  Runtime: O(n), n = char_count
			 *  @note  Memory: O(1)
			 */
			template <size_t char_count>
			bool ends_with(const char (&chars)[char_count]) const
			{
				if (char_count - 1 > size()) return false;

				for (size_t i = 0; i < char_count - 1; i++) {
					if (chars[i] != get_at_index(size() + 1 - char_count + i)) return false;
				}

				return true;
			}

			/**
			 *  @brief  Checks whether this String ends with a certain character sequence.
			 *  @param  other_string  The other String to compare the end of
			 *  this String with.
			 *  @note  Runtime: O(n), n = other_string.size()
			 *  @note  Memory: O(1)
			 */
			bool ends_with(String& other_string) const
			{
				if (other_string.size() > size()) return false;

				for (size_t i = 0; i < other_string.size(); i++) {
					if (
						other_string.get_at_index(i)
						!= get_at_index(size() - other_string.size() + i)
					) return false;
				}

				return true;
			}

			/**
			 *  @brief  Checks whether this String starts with a certain character sequence.
			 *  @param  chars  The characters to compare the start of this String with.
			 *  @note  Runtime: O(n), n = char_count
			 *  @note  Memory: O(1)
			 */
			template <size_t char_count>
			bool starts_with(const char (&chars)[char_count]) const
			{
				if (char_count - 1 > size()) return false;

				for (size_t i = 0; i < char_count - 1; i++) {
					if (chars[i] != get_at_index(i)) return false;
				}

				return true;
			}

			/**
			 *  @brief  Checks whether this String ends with a certain character sequence.
			 *  @param  other_string  The other String to compare the start of
			 *  this String with.
			 *  @note  Runtime: O(n), n = other_string.size()
			 *  @note  Memory: O(1)
			 */
			bool starts_with(String& other_string) const
			{
				if (other_string.size() > size()) return false;

				for (size_t i = 0; i < other_string.size(); i++) {
					if (other_string.get_at_index(i) != get_at_index(i))
						return false;
				}

				return true;
			}

			/**
			 *  @brief  Checks whether a certain character sequence occurs at a
			 *  given index in this String.
			 *  @param  chars  The characters to find in this String.
			 *  @param  index  The index to find the characters at in this String.
			 *  @note  Runtime: O(n), n = char_count (early returns false)
			 *  @note  Memory: O(1)
			 */
			template <size_t char_count>
			bool substring_occurs_at(const char (&chars)[char_count], size_t index) const
			{
				if (index + char_count - 1 > size()) return false;

				for (size_t i = 0; i < char_count - 1; i++) {
					if (chars[i] != get_at_index(index + i)) return false;
				}

				return true;
			}

			/**
			 *  @brief  Checks whether a certain substring occurs at a given index in
			 *  this String.
			 *  @param  substring  The substring to find in this String.
			 *  @param  index  The index to find the substring at in this String.
			 *  @note  Runtime: O(n), n = substring.size() (early returns false)
			 *  @note  Memory: O(1)
			 */
			bool substring_occurs_at(const String& substring, size_t index = 0) const
			{
				if (index + substring.size() > size()) return false;

				for (size_t i = 0; i < substring.size(); i++) {
					if (substring.get_at_index(i) != get_at_index(index + i))
						return false;
				}

				return true;
			}

			// Prevent C++ inherited class Name Hiding

			using DynamicArray::indices_of;

			/**
			 *  @brief  Returns the indices all occurrences of a given character sequence.
			 *  @param  chars  The character sequence to find the indices of.
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(n), n = found indices
			 */
			template <size_t char_count>
			DynamicArray<size_t> indices_of(const char (&chars)[char_count]) const
			{
				DynamicArray<size_t> indices;

				size_t starting_index;
				size_t j = 0;
				bool going = false;

				for (size_t i = 0; i < size(); i++) {
					if (going) {
						if (get_at_index(i) != chars[j++]) {
							j = 0;
							going = false;
						} else if (j == char_count - 1) {
							indices.append(starting_index);
							going = false;
						}
					} else if (get_at_index(i) == chars[0]) {
						j = 1;
						going = true;
						starting_index = i;

						if (j == char_count - 1) {
							indices.append(starting_index);
							going = false;
						}
					}
				}

				return indices;
			}

			/**
			 *  @brief  Returns the indices all occurrences of a given String.
			 *  @param  other_string  The String to find the indices of.
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(n), n = found indices
			 */
			DynamicArray<size_t> indices_of(String& other_string) const
			{
				DynamicArray<size_t> indices;

				size_t starting_index;
				size_t j = 0;
				bool going = false;

				for (size_t i = 0; i < size(); i++) {
					if (going) {
						if (get_at_index(i) != other_string[j++]) {
							j = 0;
							going = false;
						} else if (j == other_string.size()) {
							indices.append(starting_index);
							going = false;
						}
					} else if (get_at_index(i) == other_string[0]) {
						j = 1;
						going = true;
						starting_index = i;

						if (j == other_string.size()) {
							indices.append(starting_index);
							going = false;
						}
					}
				}

				return indices;
			}

			/**
			 *  @brief  Checks whether this String includes a certain character sequence.
			 *  @param  chars  The characters to search in this String.
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(1)
			 */
			template <size_t char_count>
			bool includes(const char (&chars)[char_count]) const
			{
				if (char_count - 1 > size()) return false;

				size_t j = 0;
				bool going = false;

				for (size_t i = 0; i < size(); i++) {
					if (going) {
						if (get_at_index(i) != chars[j++]) {
							j = 0;
							going = false;
						} else if (j == char_count - 1) {
							return true;
						}
					} else {
						if (get_at_index(i) == chars[0]) {
							j++;
							going = true;

							if (j == char_count - 1) return true;
						}
					}
				}

				return false;
			}

			/**
			 *  @brief  Checks whether this String includes another String.
			 *  @param  other_string  The String to search for.
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(1)
			 */
			bool includes(String& other_string) const
			{
				if (other_string.size() > size()) return false;

				size_t j = 0;
				bool going = false;

				for (size_t i = 0; i < size(); i++) {
					if (going) {
						if (get_at_index(i) != other_string.get_at_index(j++)) {
							j = 0;
							going = false;
						} else if (j == other_string.size()) {
							return true;
						}
					} else {
						if (get_at_index(i) == other_string[0]) {
							j++;
							going = true;

							if (j == other_string.size()) return true;
						}
					}
				}

				return false;
			}

			/**
			 *  @brief  Checks whether this String includes a certain character.
			 *  @param  character  The character to search in this String.
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(1)
			 */
			bool includes(const char character) const
			{
				for (size_t i = 0; i < size(); i++) {
					if (get_at_index(i) == character) return true;
				}

				return false;
			}

			/**
			 *  @brief  Prepends the String with a certain character, until the String
			 *  length is bigger than the given max_length value.
			 *  @param  character  The character to prepend the String with.
			 *  @param  max_length  The maximum length of the String after the padding.
			 *  If the String is already larger than this value in the first place,
			 *  it will not shrink.
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(1)
			 */
			void pad_start(char character, size_t max_length)
			{
				if (max_length <= size()) return;

				// Reserve enough space

				size_t pad_size = max_length - size();
				reserve(pad_size);

				// Shift the String to the right by the correct amount

				shift_right(pad_size);
				unsafe_increment_element_count(pad_size);

				// Insert the pad characters

				for (size_t i = 0; i < pad_size; i++) {
					set_at_index(i, character);
				}
			}

			/**
			 *  @brief  Appends the String with a certain character, until the String
			 *  length is bigger than the given max_length value.
			 *  @param  character  The character to append the String with.
			 *  @param  max_length  The maximum length of the String after padding.
			 *  If the String is already larger than this value in the first place,
			 *  it will not shrink.
			 *  @note  Runtime: O(1) if the buffer does not have to grow,
			 *  O(n) otherwise, n = size()
			 *  @note  Memory: O(1)
			 */
			void pad_end(char character, size_t max_length)
			{
				if (max_length <= size()) return;

				// Reserve enough size

				size_t pad_size = max_length - size();
				reserve(pad_size);
				unsafe_increment_element_count(pad_size);

				// Insert the pad characters

				for (size_t i = size() - pad_size; i < size(); i++) {
					set_at_index(i, character);
				}
			}

			/**
			 *  @brief  Interweaves a given character sequence between this String
			 *  at a given index in place.
			 *  @param  index  The index to append the new character sequence.
			 *  @param  chars  The character sequence to insert at the index.
			 *  @note  Runtime: O(n + m), n = size(), m = char_count
			 *  @note  Memory: O(1)
			 */
			template <size_t char_count>
			void splice(size_t index, const char (&chars)[char_count])
			{
				size_t old_size = size();
				reserve(char_count);
				unsafe_increment_element_count(char_count);

				// Move the right hand side to the right

				for (size_t i = old_size; i > index; i--) {
					set_at_index(i - 1 + char_count, get_at_index(i - 1));
				}

				// Put the character sequence in the created gap

				for (size_t i = 0; i < char_count; i++) {
					set_at_index(index + i, chars[i]);
				}
			}

			/**
			 *  @brief  Replaces all matches of a given search_character with another
			 *  character sequence
			 *  @param  search_character  The character to search.
			 *  @param  chars  The character sequence to replace each match with.
			 *  @note  Runtime: O(n + m * o), n = size(), m = char_count, o = found matches
			 *  @note  Memory: O(o)
			 */
			template <size_t char_count>
			void replace(char search_character, const char (&chars)[char_count])
			{
				// Get the indices of the found matches

				DynamicArray<size_t> found_positions = indices_of(search_character);

				// Reserve enough space and increment the size manually

				size_t extra_chars = found_positions.size() * (char_count - 2);
				reserve(extra_chars);
				unsafe_increment_element_count(extra_chars);

				// Replace each match

				size_t prev_pos = size();

				for (size_t i = found_positions.size(); i > 0; i--) {
					size_t pos = found_positions[i - 1];
					size_t offset = i * (char_count - 2);

					// Shift the part of the String

					shift_right(offset, pos + 1, prev_pos - pos - 1);

					// Put the new character sequence in the created gap

					for (size_t j = 0; j < char_count - 1; j++) {
						set_at_index(pos + (i - 1) * (char_count - 2) + j, chars[j]);
					}

					prev_pos = pos;
				}
			}

			/**
			 *  @brief  Replaces all matches of a given character sequence with
			 *  another character sequence
			 *  @param  search_chars  The character sequence to search and replace.
			 *  @param  replacement_chars  The character sequence to replace it with.
			 *  @note  Runtime: O(n + m * o), n = size(), m = search_chars length,
			 *  o = found matches
			 *  @note  Memory: O(o)
			 */
			template <size_t search_chars_count, size_t replacement_chars_count>
			void replace(
				const char (&search_chars)[search_chars_count],
				const char (&replacement_chars)[replacement_chars_count]
			)
			{
				// Get the indices of the found matches

				DynamicArray<size_t> found_positions = indices_of(search_chars);

				if (replacement_chars_count == search_chars_count) {
					// The character counts match, no shifting is necessary
					// Replace each match, left to right

					for (size_t i = 0; i < found_positions.size(); i++) {
						size_t pos = found_positions[i];

						for (size_t j = 0; j < replacement_chars_count - 1; j++) {
							set_at_index(pos + j, replacement_chars[j]);
						}
					}
				} else if (replacement_chars_count > search_chars_count) {
					// The String will grow
					// Reserve enough space and increment the size manually

					size_t size_diff = replacement_chars_count - search_chars_count;
					size_t extra_chars = found_positions.size() * size_diff;

					size_t prev_pos = size();

					reserve(extra_chars);
					unsafe_increment_element_count(extra_chars);

					// Replace each match, right to left

					for (size_t i = found_positions.size(); i > 0; i--) {
						size_t pos = found_positions[i - 1];
						size_t offset = i * size_diff;

						// Index to shift from

						size_t starting_index = pos + extra_chars - 1;

						// Length of the right shift

						size_t shift_length = prev_pos - starting_index;

						// Shift the inter part of the String to the right

						shift_right(offset, starting_index, shift_length);

						// Put the new character sequence in the created gap

						for (size_t j = 0; j < replacement_chars_count - 1; j++) {
							size_t index = pos + (i - 1) * size_diff + j;
							set_at_index(index, replacement_chars[j]);
						}

						prev_pos = pos;
					}
				} else {
					// The String will shrink

					size_t size_diff = search_chars_count - replacement_chars_count;
					size_t redundant_chars = found_positions.size() * size_diff;

					// Replace each match, left to right

					for (size_t i = 0; i < found_positions.size(); i++) {
						size_t pos = found_positions[i];
						size_t next_pos = (i == found_positions.size() - 1)
							? size() - 1
							: found_positions[i + 1];

						// Put the new character sequence straight in place

						for (size_t j = 0; j < replacement_chars_count - 1; j++) {
							size_t index = pos - i * size_diff + j;
							set_at_index(index, replacement_chars[j]);
						}

						size_t offset = (i + 1) * size_diff;

						// Index to shift from

						size_t starting_index = pos + search_chars_count - 1;

						// Length of the left shift

						size_t shift_length = next_pos - starting_index;

						// Shift the inter part of the String to the left

						shift_left(offset, starting_index, shift_length);
					}

					// Shorten the String

					unreserve(redundant_chars);
					unsafe_decrement_element_count(redundant_chars);
				}
			}

			/**
			 *  @brief  Replaces all matches of a given search_string with another
			 *  character sequence
			 *  @param  search_string  The String to search and replace.
			 *  @param  replacement_chars  The character sequence to replace it with.
			 *  @note  Runtime: O(n + m * o), n = size(), m = search_string.size(),
			 *  o = found matches
			 *  @note  Memory: O(o)
			 */
			template <size_t replacement_chars_count>
			void replace(
				String& search_string,
				const char (&replacement_chars)[replacement_chars_count]
			)
			{
				// Get the indices of the found matches

				DynamicArray<size_t> found_positions = indices_of(search_string);

				if (replacement_chars_count - 1 == search_string.size()) {
					// The string sizes match, no shifting is necessary
					// Replace each match, left to right

					for (size_t i = 0; i < found_positions.size(); i++) {
						size_t pos = found_positions[i];

						for (size_t j = 0; j < replacement_chars_count - 1; j++) {
							set_at_index(pos + j, replacement_chars[j]);
						}
					}
				} else if (replacement_chars_count - 1 > search_string.size()) {
					// The String will grow
					// Reserve enough space and increment the size manually

					size_t size_diff = replacement_chars_count - 1 - search_string.size();
					size_t extra_chars = found_positions.size() * size_diff;

					size_t prev_pos = size();

					reserve(extra_chars);
					unsafe_increment_element_count(extra_chars);

					// Replace each match, right to left

					for (size_t i = found_positions.size(); i > 0; i--) {
						size_t pos = found_positions[i - 1];
						size_t offset = i * size_diff;

						// Index to shift from

						size_t starting_index = pos + extra_chars - 1;

						// Length of the right shift

						size_t shift_length = prev_pos - starting_index;

						// Shift the inter part of the String to the right

						shift_right(offset, starting_index, shift_length);

						// Put the new character sequence in the created gap

						for (size_t j = 0; j < replacement_chars_count - 1; j++) {
							size_t index = pos + (i - 1) * size_diff + j;
							set_at_index(index, replacement_chars[j]);
						}

						prev_pos = pos;
					}
				} else {
					// The String will shrink

					size_t size_diff = search_string.size() - (replacement_chars_count - 1);
					size_t redundant_chars = found_positions.size() * size_diff;

					// Replace each match, left to right

					for (size_t i = 0; i < found_positions.size(); i++) {
						size_t pos = found_positions[i];
						size_t next_pos = (i == found_positions.size() - 1)
							? size() - 1
							: found_positions[i + 1];

						// Put the new character sequence straight in place

						for (size_t j = 0; j < replacement_chars_count - 1; j++) {
							size_t index = pos - i * size_diff + j;
							set_at_index(index, replacement_chars[j]);
						}

						size_t offset = (i + 1) * size_diff;

						// Index to shift from

						size_t starting_index = pos + search_string.size();

						// Length of the left shift

						size_t shift_length = next_pos - starting_index;

						// Shift the inter part of the String to the left

						shift_left(offset, starting_index, shift_length);
					}

					// Shorten the String

					unreserve(redundant_chars);
					unsafe_decrement_element_count(redundant_chars);
				}
			}

			/**
			 *  @brief  Replaces all matches of a given character sequence with
			 *  another replacement_string
			 *  @param  search_chars  The character sequence to search and replace.
			 *  @param  replacement_string  The String to replace it with.
			 *  @note  Runtime: O(n + m * o), n = size(), m = search_chars length,
			 *  o = found matches
			 *  @note  Memory: O(o)
			 */
			template <size_t search_chars_count>
			void replace(
				const char (&search_chars)[search_chars_count],
				String& replacement_string
			)
			{
				// Get the indices of the found matches

				DynamicArray<size_t> found_positions = indices_of(search_chars);

				if (replacement_string.size() == search_chars_count - 1) {
					// The character counts match, no shifting is necessary
					// Replace each match, left to right

					for (size_t i = 0; i < found_positions.size(); i++) {
						size_t pos = found_positions[i];

						for (size_t j = 0; j < replacement_string.size(); j++) {
							set_at_index(pos + j, replacement_string[j]);
						}
					}
				} else if (replacement_string.size() > search_chars_count - 1) {
					// The String will grow
					// Reserve enough space and increment the size manually

					size_t size_diff = replacement_string.size() - (search_chars_count - 1);
					size_t extra_chars = found_positions.size() * size_diff;

					size_t prev_pos = size();

					reserve(extra_chars);
					unsafe_increment_element_count(extra_chars);

					// Replace each match, right to left

					for (size_t i = found_positions.size(); i > 0; i--) {
						size_t pos = found_positions[i - 1];
						size_t offset = i * size_diff;

						// Index to shift from

						size_t starting_index = pos + extra_chars - 1;

						// Length of the right shift

						size_t shift_length = prev_pos - starting_index;

						// Shift the inter part of the String to the right

						shift_right(offset, starting_index, shift_length);

						// Put the new character sequence in the created gap

						for (size_t j = 0; j < replacement_string.size(); j++) {
							size_t index = pos + (i - 1) * size_diff + j;
							set_at_index(index, replacement_string[j]);
						}

						prev_pos = pos;
					}
				} else {
					// The String will shrink

					size_t size_diff = (search_chars_count - 1) - replacement_string.size();
					size_t redundant_chars = found_positions.size() * size_diff;

					// Replace each match, left to right

					for (size_t i = 0; i < found_positions.size(); i++) {
						size_t pos = found_positions[i];
						size_t next_pos = (i == found_positions.size() - 1)
							? size() - 1
							: found_positions[i + 1];

						// Put the new character sequence straight in place

						for (size_t j = 0; j < replacement_string.size(); j++) {
							size_t index = pos - i * size_diff + j;
							set_at_index(index, replacement_string[j]);
						}

						size_t offset = (i + 1) * size_diff;

						// Index to shift from

						size_t starting_index = pos + search_chars_count - 1;

						// Length of the left shift

						size_t shift_length = next_pos - starting_index;

						// Shift the inter part of the String to the left

						shift_left(offset, starting_index, shift_length);
					}

					// Shorten the String

					unreserve(redundant_chars);
					unsafe_decrement_element_count(redundant_chars);
				}
			}

			/**
			 *  @brief  Replaces all matches of a given search_string with another
			 *  replacement_string
			 *  @param  search_string  The String to search and replace.
			 *  @param  replacement_string  The String to replace it with.
			 *  @note  Runtime: O(n + m * o), n = size(), m = search_string.size(),
			 *  o = found matches
			 *  @note  Memory: O(o)
			 */
			void replace(
				String& search_string,
				String& replacement_string
			)
			{
				// Get the indices of the found matches

				DynamicArray<size_t> found_positions = indices_of(search_string);

				if (replacement_string.size() == search_string.size()) {
					// The string sizes match, no shifting is necessary
					// Replace each match, left to right

					for (size_t i = 0; i < found_positions.size(); i++) {
						size_t pos = found_positions[i];

						for (size_t j = 0; j < replacement_string.size(); j++) {
							set_at_index(pos + j, replacement_string[j]);
						}
					}
				} else if (replacement_string.size() > search_string.size()) {
					// The String will grow
					// Reserve enough space and increment the size manually

					size_t size_diff = replacement_string.size() - search_string.size();
					size_t extra_chars = found_positions.size() * size_diff;

					size_t prev_pos = size();

					reserve(extra_chars);
					unsafe_increment_element_count(extra_chars);

					// Replace each match, right to left

					for (size_t i = found_positions.size(); i > 0; i--) {
						size_t pos = found_positions[i - 1];
						size_t offset = i * size_diff;

						// Index to shift from

						size_t starting_index = pos + extra_chars - 1;

						// Length of the right shift

						size_t shift_length = prev_pos - starting_index;

						// Shift the inter part of the String to the right

						shift_right(offset, starting_index, shift_length);

						// Put the new character sequence in the created gap

						for (size_t j = 0; j < replacement_string.size(); j++) {
							size_t index = pos + (i - 1) * size_diff + j;
							set_at_index(index, replacement_string[j]);
						}

						prev_pos = pos;
					}
				} else {
					// The String will shrink

					size_t size_diff = search_string.size() - replacement_string.size();
					size_t redundant_chars = found_positions.size() * size_diff;

					// Replace each match, left to right

					for (size_t i = 0; i < found_positions.size(); i++) {
						size_t pos = found_positions[i];
						size_t next_pos = (i == found_positions.size() - 1)
							? size() - 1
							: found_positions[i + 1];

						// Put the new character sequence straight in place

						for (size_t j = 0; j < replacement_string.size(); j++) {
							size_t index = pos - i * size_diff + j;
							set_at_index(index, replacement_string[j]);
						}

						size_t offset = (i + 1) * size_diff;

						// Index to shift from

						size_t starting_index = pos + search_string.size();

						// Length of the left shift

						size_t shift_length = next_pos - starting_index;

						// Shift the inter part of the String to the left

						shift_left(offset, starting_index, shift_length);
					}

					// Shorten the String

					unreserve(redundant_chars);
					unsafe_decrement_element_count(redundant_chars);
				}
			}

			/**
			 *  @brief  Returns a new String formed by a contiguous part of this
			 *  String starting at some offset with a certain length.
			 *  @param  offset  The index of the first character to include in the
			 *  returned String.
			 *  @param  length  The length of the substring.
			 *  @note  Runtime: O(n), n = min(length, size() - offset)
			 *  @note  Memory: O(n), n = min(length, size() - offset)
			 */
			String substring(size_t offset, size_t length = SIZE_MAX) const
			{
				length = std::min(length, size() - offset);

				String str(length);
				str.unsafe_increment_element_count(length);

				for (size_t i = 0; i < length; i++) {
					str.set_at_index(i, get_at_index(offset + i));
				}

				return str;
			}

			/**
			 *  @brief  Returns a new String formed by the characters between two
			 *  given indices of this String.
			 *  @param  left_index  The index of the first character to include in
			 *  the returned String.
			 *  @param  right_index  The index of the last character to include in
			 *  the returned String (inclusive).
			 *  @note  Runtime: O(n), n = min(right_index, size()) - left_index + 1
			 *  @note  Memory: O(n), n = min(right_index, size()) - left_index + 1
			 */
			String between(size_t left_index, size_t right_index = SIZE_MAX) const
			{
				right_index = std::min(right_index, size());

				if (left_index > right_index) {
					return String(0UL);
				}

				size_t length = right_index - left_index + 1;

				String str(length);
				str.unsafe_increment_element_count(length);

				for (size_t i = 0; i < length; i++) {
					str.set_at_index(i, get_at_index(left_index + i));
				}

				return str;
			}

			/**
			 *  @brief  Splits up a String into a DynamicArray of Strings, seperated
			 *  by a given delimiter character.
			 *  @param  delimiter  The character to split on. The delimiter will be
			 *  removed in the output.
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(n), n = size()
			 */
			DynamicArray<String> split(char delimiter) const
			{
				DynamicArray<size_t> found_indices = indices_of(delimiter);

				size_t strings_size = found_indices.size() + 1;
				DynamicArray<String> strings(strings_size);
				strings.unsafe_increment_element_count(strings_size);

				for (size_t i = 0; i < strings_size; i++) {
					size_t left_index = (i == 0)
						? 0
						: found_indices[i - 1] + 1;

					size_t right_index = (i != strings_size - 1)
						? found_indices[i] - 1
						: size() - 1;

					String substr = between(left_index, right_index);
					strings[i] = substr;
				}

				return strings;
			}

			/**
			 *  @brief  Splits up a String into a DynamicArray of Strings, seperated
			 *  by a given delimiter character sequence.
			 *  @param  delimiter  The character sequence to split on. The delimiter
			 *  will be removed in the output.
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(n), n = size()
			 */
			template <size_t char_count>
			DynamicArray<String> split(const char (&delimiter)[char_count]) const
			{
				DynamicArray<size_t> found_indices = indices_of(delimiter);

				size_t strings_size = found_indices.size() + 1;
				DynamicArray<String> strings(strings_size);
				strings.unsafe_increment_element_count(strings_size);

				for (size_t i = 0; i < strings_size; i++) {
					size_t left_index = (i == 0)
						? 0
						: found_indices[i - 1] + char_count - 1;

					size_t right_index = (i != strings_size - 1)
						? found_indices[i] - 1
						: size() - 1;

					String substr = between(left_index, right_index);
					strings[i] = substr;
				}

				return strings;
			}

			/**
			 *  @brief  Splits up a String into a DynamicArray of Strings, seperated
			 *  by a given delimiter String.
			 *  @param  delimiter  The String to split on. The delimiter will be
			 *  removed in the output.
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(n), n = size()
			 */
			DynamicArray<String> split(String delimiter) const
			{
				DynamicArray<size_t> found_indices = indices_of(delimiter);

				size_t strings_size = found_indices.size() + 1;
				DynamicArray<String> strings(strings_size);
				strings.unsafe_increment_element_count(strings_size);

				for (size_t i = 0; i < strings_size; i++) {
					size_t left_index = (i == 0)
						? 0
						: found_indices[i - 1] + delimiter.size();

					size_t right_index = (i != strings_size - 1)
						? found_indices[i] - 1
						: size() - 1;

					String substr = between(left_index, right_index);
					strings[i] = substr;
				}

				return strings;
			}

			/**
			 *  @brief  Returns a new String that starts at a given index and ends
			 *  at the index of the next position of a given delimiter.
			 *  If the delimiter is not found, the rest of the String is returned.
			 *  @param  delimiter  The character to bound the returned String on.
			 *  @param  index  The starting index of the returned String.
			 *  @note  Runtime: O(n), n = delim_index - index
			 *  @note  Memory: O(n), n = delim_index - index
			 */
			String delimit(char delimiter, size_t index = 0) const
			{
				size_t right_index = size() - 1;

				for (size_t i = index; i < size() - 1; i++) {
					if (get_at_index(i) == delimiter) {
						right_index = i - 1;
						break;
					}
				}

				return between(index, right_index);
			}

			/**
			 *  @brief  Returns a new String that starts at a given index and ends
			 *  at the index of the next position of a given delimiter.
			 *  If the delimiter is not found, the rest of the String is returned.
			 *  @param  delimiter  The character sequence to bound the returned String on.
			 *  @param  index  The starting index of the returned String.
			 *  @note  Runtime: O(n), n = delim_index - index
			 *  @note  Memory: O(n), n = delim_index - index
			 */
			template <size_t delimeter_len>
			String delimit(const char (&delimiter)[delimeter_len], size_t index = 0) const
			{
				size_t right_index = size() - 1;

				for (size_t i = index; i < size() - 1; i++) {
					if (substring_occurs_at(delimiter, i)) {
						right_index = i - 1;
						break;
					}
				}

				return between(index, right_index);
			}

			/**
			 *  @brief  Returns a new String that starts at a given index and ends
			 *  at the index of the next position of a given delimiter.
			 *  If the delimiter is not found, the rest of the String is returned.
			 *  @param  delimiter  The String to bound the returned String on.
			 *  @param  index  The starting index of the returned String.
			 *  @note  Runtime: O(n), n = delim_index - index
			 *  @note  Memory: O(n), n = delim_index - index
			 */
			String delimit(const String& delimiter, size_t index = 0) const
			{
				size_t right_index = size() - 1;

				for (size_t i = index; i < size() - 1; i++) {
					if (substring_occurs_at(delimiter, i)) {
						right_index = i - 1;
						break;
					}
				}

				return between(index, right_index);
			}

			/**
			 *  @brief  Replaces all lowercase characters [a-z] with their
			 *  corresponding capital representation [A-Z].
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(n), n = size()
			 */
			void transform_to_uppercase()
			{
				for (size_t i = 0; i < size(); i++) {
					char c = get_at_index(i);
					if (c >= 'a' && c <= 'z') set_at_index(i, c - 32);
				}
			}

			/**
			 *  @brief  Replaces all uppercase characters [A-Z] with their
			 *  corresponding lowercase representation [a-z].
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(n), n = size()
			 */
			void transform_to_lowercase()
			{
				for (size_t i = 0; i < size(); i++) {
					char c = get_at_index(i);
					if (c >= 'A' && c <= 'Z') set_at_index(i, c + 32);
				}
			}

			/**
			 *  @brief  Formats a String
			 */
			static String format(const char* fmt, ...)
			{
				// Size calculation

				va_list args;
				va_start(args, fmt);

				size_t size = 0;
				size_t i = 0;

				#define SIZE_8  0
				#define SIZE_16 1
				#define SIZE_32 2
				#define SIZE_64 3

				#define err(message, ...) \
					fprintf(stderr, message, ##__VA_ARGS__); \
					exit(1)

				uint8_t length_modifier = SIZE_32;
				bool length_modifier_set = false;
				char pad_char = '\0';
				size_t pad_size = 0;
				size_t precision = 6;
				bool precision_modifier_set = false;
				size_t num_size;

				while (fmt[i] != '\0') {
					if (fmt[i] == '%') {
						continue_specifier:
						i++;

						switch (fmt[i]) {
							case '%':
								size++;
									goto end_specifier;

							case 'd':
							case 'i':
								switch (length_modifier) {
									case SIZE_8:
										// Max length is 4 ("-128")

										va_arg(args, int32_t);
										num_size = 4;
										break;

									case SIZE_16:
										// Max length is 6 ("-32768")

										va_arg(args, int32_t);
										num_size = 6;
										break;

									case SIZE_32:
										// Max length is 11 ("-2147483648")

										va_arg(args, int32_t);
										num_size = 11;
										break;

									case SIZE_64:
										// Max length is 20 ("-9223372036854775808")

										va_arg(args, int64_t);
										num_size = 20;
										break;
								}

								size += std::max(num_size, pad_size);
								goto end_specifier;

							case 'u':
								switch (length_modifier) {
									case SIZE_8:
										// Max length is 3 ("256")

										va_arg(args, uint32_t);
										num_size = 3;
										break;

									case SIZE_16:
										// Max length is 5 ("65536")

										va_arg(args, uint32_t);
										num_size = 5;
										break;

									case SIZE_32:
										// Max length is 10 ("4294967296")

										va_arg(args, uint32_t);
										num_size = 10;
										break;

									case SIZE_64:
										// Max length is 20 ("18446744073709551616")

										va_arg(args, uint64_t);
										num_size = 20;
										break;
								}

								size += std::max(num_size, pad_size);
								goto end_specifier;

							case 'o':
								switch (length_modifier) {
									case SIZE_8:
										// Max length is 5 ("0o400")

										va_arg(args, uint32_t);
										num_size = 5;
										break;

									case SIZE_16:
										// Max length is 8 ("0o200000")

										va_arg(args, uint32_t);
										num_size = 8;
										break;

									case SIZE_32:
										// Max length is 13 ("0o40000000000")

										va_arg(args, uint32_t);
										num_size = 13;
										break;

									case SIZE_64:
										// Max length is 24 ("0o2000000000000000000000")

										va_arg(args, uint64_t);
										num_size = 24;
										break;
								}

								size += std::max(num_size, pad_size);
								goto end_specifier;

							case 'x':
							case 'X':
								switch (length_modifier) {
									case SIZE_8:
										// Max length is 5 ("0x100")

										va_arg(args, uint32_t);
										num_size = 5;
										break;

									case SIZE_16:
										// Max length is 7 ("0x10000")

										va_arg(args, uint32_t);
										num_size = 7;
										break;

									case SIZE_32:
										// Max length is 11 ("0x100000000")

										va_arg(args, uint32_t);
										num_size = 11;
										break;

									case SIZE_64:
										// Max length is 19 ("0x10000000000000000")

										va_arg(args, uint64_t);
										num_size = 19;
										break;
								}

								size += std::max(num_size, pad_size);
								goto end_specifier;

							case 'f':
								switch (length_modifier) {
									case SIZE_8:
									case SIZE_16:
										err("No support for 8 or 16-bit floating point numbers");

									case SIZE_32:
									case SIZE_64:
									{
										double num = va_arg(args, double);
										num_size = std::max(log10(abs(num)) + 1, 1.0) + 1;
										break;
									}
								}

								// Fraction digits

								if (precision) num_size += precision + 1; // '.' character
								size += std::max(num_size, pad_size);
								goto end_specifier;

							case 'c':
							{
								char c = va_arg(args, int);
								size += 1;
								goto end_specifier;
							}

							case 's':
							{
								const char *char_arr_arg = va_arg(args, const char *);

								if (precision_modifier_set) {
									size += std::min(precision, strlen(char_arr_arg));
								} else {
									size += strlen(char_arr_arg);
								}

								goto end_specifier;
							}

							case 'S':
							{
								String& str_arg = va_arg(args, String);

								if (precision_modifier_set) {
									size += std::min(precision, str_arg.size());
								} else {
									size += str_arg.size();
								}

								goto end_specifier;
							}

							case '0':
							{
								if (pad_char) {
									err("Cannot set padding modifier twice (0)\n");
								}

								pad_char = '0';
								flow_tools::UintFromStr num_and_len =
									flow_tools::read_uint_from_str(fmt + i + 1);
								i += num_and_len.len;

								pad_size = num_and_len.val;

								goto continue_specifier;
							}

							case '-':
							{
								if (pad_char) {
									err("Cannot set padding modifier twice (-)\n");
								}

								pad_char = ' ';

								flow_tools::UintFromStr num_and_len =
									flow_tools::read_uint_from_str(fmt + i + 1);
								i += num_and_len.len;

								pad_size = num_and_len.val;

								goto continue_specifier;
							}

							case '.':
							{
								if (precision_modifier_set) {
									err("Cannot set precision modifier twice (.)\n");
								}

								precision_modifier_set = true;

								flow_tools::UintFromStr num_and_len =
									flow_tools::read_uint_from_str(fmt + i + 1);
								i += num_and_len.len;

								precision = num_and_len.val;

								goto continue_specifier;
							}

							case 'h':
								if (length_modifier_set) {
									err("Cannot set length modifier twice\n");
								}

								length_modifier_set = true;

								if (fmt[i + 1] == 'h') {
									i++;
									length_modifier = SIZE_8;
								} else {
									length_modifier = SIZE_16;
								}

								goto continue_specifier;

							case 'l':
								if (length_modifier_set) {
									err("Cannot set length modifier twice\n");
								}

								length_modifier_set = true;

								if (fmt[i + 1] == 'l') {
									i++;
									length_modifier = SIZE_64;
								} else {
									length_modifier = SIZE_32;
								}

								goto continue_specifier;

							default:
								err("Unknown modifier %c passed to String::format()\n",
									fmt[i]);
						}
					} else {
						size++;
					}

					end_specifier:
					length_modifier = SIZE_32;
					length_modifier_set = false;
					pad_char = '\0';
					pad_size = 0;
					precision = 6;
					precision_modifier_set = false;
					i++;
				}

				String str(size);

				va_end(args);
				va_start(args, fmt);

				i = 0;
				char *buf = str.data();
				size_t buf_offset = 0;

				while (fmt[i] != '\0') {
					if (fmt[i] == '%') {
						continue_specifier_1:
						i++;

						switch (fmt[i]) {
							case '%':
								buf[buf_offset++] = '%';
								goto end_specifier_1;

							case 'd':
							case 'i':
								switch (length_modifier) {
									case SIZE_8:
									{
										int8_t num = va_arg(args, int32_t);
										buf_offset += flow_tools::write_int_to_str(
											num, buf + buf_offset, pad_char, pad_size);

										goto end_specifier_1;
									}

									case SIZE_16:
									{
										int16_t num = va_arg(args, int32_t);
										buf_offset += flow_tools::write_int_to_str(
											num, buf + buf_offset, pad_char, pad_size);
										goto end_specifier_1;
									}

									case SIZE_32:
									{
										int32_t num = va_arg(args, int32_t);
										buf_offset += flow_tools::write_int_to_str(
											num, buf + buf_offset, pad_char, pad_size);
										goto end_specifier_1;
									}

									case SIZE_64:
									{
										int64_t num = va_arg(args, int64_t);
										buf_offset += flow_tools::write_int_to_str(
											num, buf + buf_offset, pad_char, pad_size);
										goto end_specifier_1;
									}
								}

							case 'u':
								switch (length_modifier) {
									case SIZE_8:
									{
										uint8_t num = va_arg(args, uint32_t);
										buf_offset += flow_tools::write_uint_to_str(
											num, buf + buf_offset, pad_char, pad_size);
										goto end_specifier_1;
									}

									case SIZE_16:
									{
										uint16_t num = va_arg(args, uint32_t);
										buf_offset += flow_tools::write_uint_to_str(
											num, buf + buf_offset, pad_char, pad_size);
										goto end_specifier_1;
									}

									case SIZE_32:
									{
										uint32_t num = va_arg(args, uint32_t);
										buf_offset += flow_tools::write_uint_to_str(
											num, buf + buf_offset, pad_char, pad_size);
										goto end_specifier_1;
									}

									case SIZE_64:
									{
										uint64_t num = va_arg(args, uint64_t);
										buf_offset += flow_tools::write_uint_to_str(
											num, buf + buf_offset, pad_char, pad_size);
										goto end_specifier_1;
									}
								}

							case 'o':
								switch (length_modifier) {
									case SIZE_8:
									{
										uint8_t num = va_arg(args, uint32_t);
										buf[buf_offset] = '0';
										buf[buf_offset + 1] = 'o';
										buf_offset += 2;
										buf_offset += flow_tools::write_uint_to_str<8>(
											num, buf + buf_offset, pad_char, pad_size);
										goto end_specifier_1;
									}

									case SIZE_16:
									{
										uint16_t num = va_arg(args, uint32_t);
										buf[buf_offset] = '0';
										buf[buf_offset + 1] = 'o';
										buf_offset += 2;
										buf_offset += flow_tools::write_uint_to_str<8>(
											num, buf + buf_offset, pad_char, pad_size);
										goto end_specifier_1;
									}

									case SIZE_32:
									{
										uint32_t num = va_arg(args, uint32_t);
										buf[buf_offset] = '0';
										buf[buf_offset + 1] = 'o';
										buf_offset += 2;
										buf_offset += flow_tools::write_uint_to_str<8>(
											num, buf + buf_offset, pad_char, pad_size);
										goto end_specifier_1;
									}

									case SIZE_64:
									{
										uint64_t num = va_arg(args, uint64_t);
										buf[buf_offset] = '0';
										buf[buf_offset + 1] = 'o';
										buf_offset += 2;
										buf_offset += flow_tools::write_uint_to_str<8>(
											num, buf + buf_offset, pad_char, pad_size);
										goto end_specifier_1;
									}
								}

							case 'x':
							case 'X':
								switch (length_modifier) {
									case SIZE_8:
									{
										uint8_t num = va_arg(args, uint32_t);
										buf[buf_offset] = '0';
										buf[buf_offset + 1] = 'x';
										buf_offset += 2;
										buf_offset += flow_tools::write_uint_to_str<16>(
											num, buf + buf_offset, pad_char, pad_size);
										goto end_specifier_1;
									}

									case SIZE_16:
									{
										uint16_t num = va_arg(args, uint32_t);
										buf[buf_offset] = '0';
										buf[buf_offset + 1] = 'x';
										buf_offset += 2;
										buf_offset += flow_tools::write_uint_to_str<16>(
											num, buf + buf_offset, pad_char, pad_size);
										goto end_specifier_1;
									}

									case SIZE_32:
									{
										uint32_t num = va_arg(args, uint32_t);
										buf[buf_offset] = '0';
										buf[buf_offset + 1] = 'x';
										buf_offset += 2;
										buf_offset += flow_tools::write_uint_to_str<16>(
											num, buf + buf_offset, pad_char, pad_size);
										goto end_specifier_1;
									}

									case SIZE_64:
									{
										uint64_t num = va_arg(args, uint64_t);
										buf[buf_offset] = '0';
										buf[buf_offset + 1] = 'x';
										buf_offset += 2;
										buf_offset += flow_tools::write_uint_to_str<16>(
											num, buf + buf_offset, pad_char, pad_size);
										goto end_specifier_1;
									}
								}

							case 'f':
								switch (length_modifier) {
									case SIZE_32:
									case SIZE_64:
									{
										double num = va_arg(args, double);
										buf_offset += flow_tools::write_float_to_str(
											num, buf + buf_offset, precision);
										goto end_specifier_1;
									}
								}

							case 'c':
							{
								char c = va_arg(args, int);
								buf[buf_offset++] = c;

								goto end_specifier_1;
							}

							case 's':
							{
								const char *str = va_arg(args, const char *);
								size_t str_size = strlen(str);

								if (precision_modifier_set)
									str_size = std::min(str_size, precision);

								ssize_t padding = pad_size - str_size;
								padding = std::max(padding, (ssize_t) 0);

								memset(buf + buf_offset, pad_char, padding);
								buf_offset += padding;

								memcpy(buf + buf_offset, str, str_size);
								buf_offset += str_size;

								goto end_specifier_1;
							}

							case 'S':
							{
								String& str_arg = va_arg(args, String);
								size_t str_size = str_arg.size();

								if (precision_modifier_set)
									str_size = std::min(str_size, precision);

								ssize_t padding = pad_size - str_size;
								padding = std::max(padding, (ssize_t) 0);

								memset(buf + buf_offset, pad_char, padding);
								buf_offset += padding;

								memcpy(buf + buf_offset, str_arg.data(), str_size);
								buf_offset += str_size;

								goto end_specifier_1;
							}

							case '0':
							{
								pad_char = '0';

								flow_tools::UintFromStr num_and_len =
									flow_tools::read_uint_from_str(fmt + i + 1);
								i += num_and_len.len;

								pad_size = num_and_len.val;

								goto continue_specifier_1;
							}

							case '-':
							{
								pad_char = ' ';

								flow_tools::UintFromStr num_and_len =
									flow_tools::read_uint_from_str(fmt + i + 1);
								i += num_and_len.len;

								pad_size = num_and_len.val;

								goto continue_specifier_1;
							}

							case '.':
							{
								precision_modifier_set = true;

								flow_tools::UintFromStr num_and_len =
									flow_tools::read_uint_from_str(fmt + i + 1);
								i += num_and_len.len;

								precision = num_and_len.val;

								goto continue_specifier_1;
							}

							case 'h':
								if (fmt[i + 1] == 'h') {
									i++;
									length_modifier = SIZE_8;
								} else {
									length_modifier = SIZE_16;
								}

								goto continue_specifier_1;

							case 'l':
								if (fmt[i + 1] == 'l') {
									i++;
									length_modifier = SIZE_64;
								} else {
									length_modifier = SIZE_32;
								}

								goto continue_specifier_1;
						}
					} else {
						buf[buf_offset++] = fmt[i];
					}

					end_specifier_1:
					length_modifier = SIZE_32;
					pad_char = '\0';
					pad_size = 0;
					precision = 6;
					i++;
				}

				va_end(args);

				str.unsafe_increment_element_count(buf_offset);
				return str;
			}

			/**
			 *  @brief  Prints this String to a stream, followed by a newline.
			 *  @param  stream  The stream to print to. Defaults to stdout.
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(1)
			 */
			void print(FILE *stream = stdout)
			{
				for (size_t i = 0; i < size(); i++) {
					putc(get_at_index(i), stream);
				}

				putc('\n', stream);
			}

			static String from_num(uint8_t  num) { return String::format("%hhu",  num); }
			static String from_num(int8_t   num) { return String::format("%hhd",  num); }
			static String from_num(uint16_t num) { return String::format("%hu",   num); }
			static String from_num(int16_t  num) { return String::format("%hd",   num); }
			static String from_num(uint32_t num) { return String::format("%lu",   num); }
			static String from_num(int32_t  num) { return String::format("%ld",   num); }
			static String from_num(uint64_t num) { return String::format("%llu",  num); }
			static String from_num(int64_t  num) { return String::format("%lld",  num); }
			static String from_num(float    num) { return String::format("%.6f",  num); }
			static String from_num(double   num) { return String::format("%.15f", num); }

			/**
			 *  @brief  Allocates a String with a buffer of a certain size.
			 *  The element count will be set to the certain size.
			 *  Watch out for undefined behaviour.
			 *  @param  size  The size of the String buffer.
			 */
			static String alloc(size_t size)
			{
				String str(size);
				str.unsafe_increment_element_count(size);
				return str;
			}
	};
};

/**
 *  @brief  Calculates the hash of a String, used for hash maps etc.
 */
template <>
struct std::hash<flow::String> {
	size_t operator()(const flow::String& str) const
	{
		#if SIZE_MAX == 0xFFFFFFFF
		size_t hash = 443569081;
		#else
		size_t hash = 536003262777797;
		#endif

		size_t i = 0;
		uint8_t j = 0;
		ssize_t size = str.size();

		while (size > 0) {
			switch (size) {
				default:
				#if SIZE_MAX == 0xFFFFFFFFFFFFFFFF
				case 8:
					hash ^= (size_t) str.get_at_index(i++) << 56;
				case 7:
					hash ^= (size_t) str.get_at_index(i++) << 48;
				case 6:
					hash ^= (size_t) str.get_at_index(i++) << 40;
				case 5:
					hash ^= (size_t) str.get_at_index(i++) << 32;
				#endif
				case 4:
					hash ^= (size_t) str.get_at_index(i++) << 24;
				case 3:
					hash ^= (size_t) str.get_at_index(i++) << 16;
				case 2:
					hash ^= (size_t) str.get_at_index(i++) << 8;
				case 1:
					hash ^= (size_t) str.get_at_index(i++);
			}

			#if SIZE_MAX == 0xFFFFFFFF
			hash *= 858371219;
			size -= 4;
			#else
			size -= 8;
			hash *= 266162830675097;
			#endif
		}

		return hash;
	}
};


#endif