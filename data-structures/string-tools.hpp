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
	uint8_t write_uint_to_str(uintx_t num, char *buf,
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
	uint8_t write_int_to_str(intx_t num, char *buf,
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

		for (size_t i = 0; i < remaining_pad_size - is_signed; i++) {
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
};

#endif