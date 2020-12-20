#ifndef FLOW_STRING_HEADER
#define FLOW_STRING_HEADER

#include <bits/stdc++.h>

#include "dynamic-array.hpp"

using namespace std;

namespace flow {
	/**
	 *  @brief  A flexible string of characters.
	 */
	class String : public DynamicArray<char> {
		public:
			/**
			 *  @brief  Creates a String from a sequence of characters.
			 */
			String(size_t size = 16) : DynamicArray<char>(size) {}

			/**
			 *  @brief  Creates a String from a sequence of characters.
			 */
			template <size_t char_count>
			String(const char (&chars)[char_count]) : DynamicArray<char>(char_count - 1)
			{
				for (size_t i = 0; i < char_count - 1; i++) {
					append(chars[i]);
				}
			}

			/**
			 *  @brief  Attaches a sequence of characters to the end of this String.
			 *  The String will automatically grow to a power of 2 if needed.
			 *  @param  chars  The sequence of characters to attach to this String.
			 *  @note  Runtime: O(n), n = char_count
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
			 *  @brief  Attaches a sequence of characters to the end of this String.
			 *  The String will automatically grow to a power of 2 if needed.
			 *  @param  chars  The sequence of characters to attach to this String.
			 *  @note  Runtime: O(n), n = char_count
			 *  @note  Memory: O(1)
			 */
			template <size_t char_count>
			void operator+=(const char (&chars)[char_count])
			{
				attach(chars);
			}

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
			String concatenate(String& other_string)
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
			String operator+(String& other_string)
			{
				return concatenate(other_string);
			}

			/**
			 *  @brief  Creates a new String by repeating this String a certain
			 *  amount of times.
			 *  @param  repeat_count  The number of times to repeat this String.
			 *  @note  Runtime: O(n * m), n = size(), m = repeat_count
			 *  @note  Memory: O(n * m)
			 */
			String duplicate(size_t repeat_count)
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
			String operator*(size_t repeat_count)
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
			bool ends_with(const char (&chars)[char_count])
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
			bool ends_with(String& other_string)
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
			bool starts_with(const char (&chars)[char_count])
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
			bool starts_with(String& other_string)
			{
				if (other_string.size() > size()) return false;

				for (size_t i = 0; i < other_string.size(); i++) {
					if (other_string.get_at_index(i) != get_at_index(i))
						return false;
				}

				return true;
			}

			/**
			 *  @brief  Returns the indices all occurrences of a given character sequence.
			 *  @param  chars  The character sequence to find the indices of.
			 *  @note  Runtime: O(n), n = size()
			 *  @note  Memory: O(n), n = found indices
			 */
			template <size_t char_count>
			DynamicArray<size_t> find_indices_of(const char (&chars)[char_count])
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
			DynamicArray<size_t> find_indices_of(String& other_string)
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
			bool includes(const char (&chars)[char_count])
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
			bool includes(String& other_string)
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
			bool includes(const char character)
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
			 *  @brief  Replaces all matches of a given search_string with another
			 *  character sequence
			 *  @param  search_string  The String to search and replace.
			 *  @param  chars  The character sequence to replace it with.
			 *  @note  Runtime: O(n + m * o), n = size(), m = search_string.size(),
			 *  o = found matches
			 *  @note  Memory: O(o)
			 */
			template <size_t char_count>
			void replace(String& search_string, const char (&chars)[char_count])
			{
				// Get the indices of the found matches

				DynamicArray<size_t> found_positions = find_indices_of(search_string);

				// Reserve enough size and increment the size manually

				if (char_count - 1 >= search_string.size()) {
					// The String will grow or stay the same
					// Reserve enough space and increment the size manually

					size_t extra_chars = found_positions.size() * (
						char_count - 1 - search_string.size()
					);

					reserve(extra_chars);
					unsafe_increment_element_count(extra_chars);

					// Replace each match

					size_t prev_pos = size();

					for (size_t i = found_positions.size(); i > 0; i--) {
						size_t pos = found_positions[i - 1];
						size_t offset = i * (char_count - 1 - search_string.size());

						// Shift the part of the String
						// Todo: fix length
						shift_right(offset, pos + 1, prev_pos - pos + 1);

						// Put the new character sequence in the created gap

						for (size_t j = 0; j < char_count - 1; j++) {
							size_t index = pos + (i - 1) * (
								char_count - 1 - search_string.size()
							) + j;
							printf("moving %ld (%c) into %ld\n", j, chars[j], index);
							printf("old char was %c\n", get_at_index(index));

							set_at_index(index, chars[j]);
							printf("new char is %c\n", get_at_index(index));
						}

						prev_pos = pos;
					}
				} else {
					// The String will shrink

					throw "NOT IMPLEMENTED";
				}
			}

			// Todo: replace, substring, split, to_uppercase, to_lowercase

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
	};
};

#endif