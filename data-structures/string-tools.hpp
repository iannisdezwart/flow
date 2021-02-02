#ifndef FLOW_STRING_TOOLS_HEADER
#define FLOW_STRING_TOOLS_HEADER

#include <bits/stdc++.h>

using namespace std;

namespace flow_tools {
	template <char starting_alphabetical_digit = 'A'>
	char get_base_digit(uint8_t num) {
		if (num < 10) {
			return '0' + num;
		}

		return starting_alphabetical_digit - 10 + num;
	}

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

	template <uint8_t base = 10, typename intx_t>
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
			char digit = get_base_digit(multiplier * (num % base));
			buf[i - 1] = digit;
			num /= base;
		}

		return length + remaining_pad_size + (is_signed ? 1 : 0);
	}

	struct UintFromStr {
		size_t val;
		size_t len;
	};

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

	template <size_t size>
	size_t copy_str(const char (&source)[size], char *dest)
	{
		for (size_t i = 0; i < size - 1; i++) {
			*dest++ = source[i];
		}

		return size - 1;
	}

	template <typename float_t>
	size_t write_float_to_str(float_t num, char *buf, uint8_t fraction_digits,
		char pad_char = '\0', size_t pad_size = 0)
	{
		if (isnan(num)) return copy_str("NaN", buf);
		if (isinf(num)) return copy_str("Inf", buf);

		size_t length = 0;

		if (num < 0) {
			*buf++ = '-';
			length += 1;
			num = -num;
		}

		int exp = max(floor(log10(num)), (float_t) 0);

		while (exp >= -fraction_digits) {
			double digit_place = exp10(exp);
			uint8_t digit = floor(num / digit_place) + FLT_EPSILON;

			// printf("digit_place = %.15f, digit = %hhu, num =  %.15f\n",
			// 	digit_place, digit, num);

			num -= digit * digit_place;
			*buf++ = '0' + digit;

			if (exp == 0 && fraction_digits != 0) {
				*buf++ = '.';
				length++;
			}

			length++;
			exp--;
		}

		// Todo: round numbers correctly

		return length;
	}
};

#endif