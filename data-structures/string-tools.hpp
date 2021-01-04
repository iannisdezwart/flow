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
	uint8_t write_uint_to_str(uintx_t num, char *buf)
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

		for (size_t i = length; i != 0; i--) {
			char digit = get_base_digit<starting_alphabetical_digit>(num % base);
			buf[i - 1] = digit;
			num /= base;
		}

		return length;
	}

	template <uint8_t base = 10, typename intx_t>
	uint8_t write_int_to_str(intx_t num, char *buf)
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

		for (size_t i = length; i != 0; i--) {
			char digit = get_base_digit(multiplier * (num % base));
			buf[i - 1] = digit;
			num /= base;
		}

		return length + (is_signed ? 1 : 0);
	}
};

#endif