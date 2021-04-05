#ifndef FLOW_STRING_TOOLS_HEADER
#define FLOW_STRING_TOOLS_HEADER

#include <bits/stdc++.h>

namespace flow_tools {
	template <char starting_alphabetical_digit = 'A'>
	char get_base_digit(uint8_t num) {
		if (num < 10) {
			return '0' + num;
		}

		return starting_alphabetical_digit - 10 + num;
	}

	/**
	 *  @brief  Writes an unsigned integer to a string. Does not check if
	 *  there is enough space left on the buffer.
	 *  @tparam  base  In what number system to display the number, defaults to 10.
	 *  @tparam  uintx_t  The type of the number to write.
	 *  @tparam  starting_alphabetical_digit  'a' or 'A', defaults to 'A'.
	 *  Used to indicate whether base 11+ numbers should be written with
	 *  upper or lower case letters.
	 *  @param  num  The number to write.
	 *  @param  buf  The character buffer to write the number to.
	 *  @param  pad_char  The character used for left padding.
	 *  @param  pad_size  The minimum length of the displayed number. If the
	 *  length of the number turns out to be smaller than this, the pad_char is
	 *  padded at the left of the string to make the number fit the desired
	 *  minimum length.
	 *  @returns  The total number of bytes written to the buffer.
	 */
	template <uint8_t base = 10, typename uintx_t,
		char starting_alphabetical_digit = 'A'>
	size_t write_uint_to_str(uintx_t num, char *buf,
		char pad_char = '\0', size_t pad_size = 0)
	{
		if (num == 0) {
			*buf = '0';
			return 1;
		}

		uintx_t n = num;
		size_t length = 0;

		while (n != 0) {
			length++;
			n /= base;
		}

		size_t remaining_pad_size;

		if (pad_size > length) remaining_pad_size = pad_size - length;
		else remaining_pad_size = 0;

		for (size_t i = 0; i < remaining_pad_size; i++) {
			*buf++ = pad_char;
		}

		for (size_t i = length; i != 0; i--) {
			char digit = get_base_digit<starting_alphabetical_digit>(num % base);
			buf[i - 1] = digit;
			num /= base;
		}

		return length + remaining_pad_size;
	}

	/**
	 *  @brief  Writes a signed integer to a string. Does not check if
	 *  there is enough space left on the buffer.
	 *  @tparam  base  In what number system to display the number, defaults to 10.
	 *  @tparam  intx_t  The type of the number to write.
	 *  @tparam  starting_alphabetical_digit  'a' or 'A', defaults to 'A'.
	 *  Used to indicate whether base 11+ numbers should be written with
	 *  upper or lower case letters.
	 *  @param  num  The number to write.
	 *  @param  buf  The character buffer to write the number to.
	 *  @param  pad_char  The character used for left padding.
	 *  @param  pad_size  The minimum length of the displayed number. If the
	 *  length of the number turns out to be smaller than this, the pad_char is
	 *  padded at the left of the string to make the number fit the desired
	 *  minimum length.
	 *  @returns  The total number of bytes written to the buffer.
	 */
	template <uint8_t base = 10, typename intx_t,
		char starting_alphabetical_digit = 'A'>
	size_t write_int_to_str(intx_t num, char *buf,
		char pad_char = '\0', size_t pad_size = 0)
	{
		if (num == 0) {
			*buf = '0';
			return 1;
		}

		constexpr intx_t sign_bit = (intx_t) 1 << (sizeof(intx_t) * 8 - 1);
		bool is_signed = num & sign_bit;
		int8_t multiplier = 1;

		if (is_signed) {
			*buf++ = '-';
			num = ~num + 1;
		}

		if (is_signed && (num ^ sign_bit) == 0) {
			multiplier = -1;
		}

		size_t length = 0;
		intx_t n = num;

		while (n != 0) {
			length++;
			n /= base;
		}

		size_t remaining_pad_size;

		if (pad_size > length) remaining_pad_size = pad_size - length;
		else remaining_pad_size = 0;
		if (remaining_pad_size != 0 && is_signed) remaining_pad_size--;

		for (size_t i = 0; i < remaining_pad_size; i++) {
			*buf++ = pad_char;
		}

		for (size_t i = length; i != 0; i--) {
			char digit = get_base_digit<starting_alphabetical_digit>(
				multiplier * (num % base));
			buf[i - 1] = digit;
			num /= base;
		}

		return length + remaining_pad_size + (is_signed ? 1 : 0);
	}

	struct UintFromStr {
		uint64_t val;
		size_t len;
	};

	/**
	 *  @brief  Reads an unsigned integer in base 10 format from a string.
	 *  Expects the character at the start of the buffer to be a base 10 digit.
	 *  @param  buf  The character buffer to read the unsigned integer from.
	 *  @returns  A UintFromStr structure, containing the value of the number
	 *  in and the length of the read number in number of bytes.
	 */
	UintFromStr read_uint_from_str(const char *buf)
	{
		UintFromStr num_and_len = { 0, 0 };

		while (*buf >= '0' && *buf <= '9') {
			num_and_len.val *= 10;
			num_and_len.val += *buf++ - '0';
			num_and_len.len++;
		}

		return num_and_len;
	}

	/**
	 *  @brief  Copies a string literal to a destination buffer.
	 *  The terminating null byte is not copied.
	 *  @tparam  size  The size of the source string including the null byte.
	 *  @param  source  A string literal used as source.
	 *  @param  dest  A pointer to the destination string buffer.
	 *  @returns  The number of bytes copied, which equals to size - 1.
	 */
	template <size_t size>
	size_t copy_str(const char (&source)[size], char *dest)
	{
		for (size_t i = 0; i < size - 1; i++) {
			*dest++ = source[i];
		}

		return size - 1;
	}

	/**
	 *  @brief  Writes a floating point number to a string. Does not check if
	 *  there is enough space left on the buffer. If the number is not a number
	 *  according to the IEEE 754 standard, "NaN" is written to the buffer.
	 *  Infinities will be written as "Inf".
	 *  @tparam  float_t  The type of the floating point number to write.
	 *  @param  num  The number to write.
	 *  @param  buf  The character buffer to write the number to.
	 *  @param  fraction_digits  The number of digits after the decimal
	 *  separator to display. The number is rounded accordingly.
	 *  @param  pad_char  The character used for left padding.
	 *  @param  pad_size  The minimum length of the displayed number. If the
	 *  length of the number turns out to be smaller than this, the pad_char is
	 *  padded at the left of the string to make the number fit the desired
	 *  minimum length.
	 *  @returns  The total number of bytes written to the buffer.
	 */
	template <typename float_t>
	size_t write_float_to_str(float_t num, char *buf, uint8_t fraction_digits,
		char pad_char = '\0', size_t pad_size = 0)
	{
		if (std::isnan(num)) return copy_str("NaN", buf);
		if (std::isinf(num)) return copy_str("Inf", buf);

		size_t length = 0;

		if (num < 0) {
			*buf++ = '-';
			length += 1;
			num = -num;
		}

		float_t n = num;
		int exp = std::max(floor(log10(num)), (float_t) 0);
		int e = exp;
		uint8_t rounding_digit;
		double digit_place;

		while (e >= -fraction_digits - 1) {
			digit_place = exp10(e);
			rounding_digit = floor(n / digit_place);

			n -= rounding_digit * digit_place;
			e--;
		}

		if (rounding_digit >= 5) {
			num += 5 * digit_place;
		}

		n = num;
		e = exp;

		while (e >= -fraction_digits) {
			digit_place = exp10(e);
			uint8_t digit = floor(n / digit_place);

			n -= digit * digit_place;
			*buf++ = '0' + digit;

			if (e == 0 && fraction_digits != 0) {
				*buf++ = '.';
				length++;
			}

			length++;
			e--;
		}

		return length;
	}
};

#endif