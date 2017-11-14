#pragma once

#include "utility.hpp"

namespace tiny_os {

#ifndef STREND
#define STREND '\0'
#endif

#ifndef NULL
#define NULL 0
#endif

size_type strlen(const char* str) {
	if (!str) return 0;
	size_type i = 0;
	while (str[i] != STREND) ++i;
	return i;
}

void strcpy(const char* source, const size_type source_length, char*& destination, const size_type destination_length) {
	for (size_type i = 0; i < destination_length && i < source_length; ++i) {
		destination[i] = source[i];
	}
}

void strcpy(const char* source, char*& destination) { strcpy(source, strlen(source), destination, strlen(destination)); }

void strrev(char* str, size_type begin, size_type end) {
	if (!str) return;
	while (begin < end) swap(str[begin++], str[end--]);
}

void strrev(char* str) { auto end = strlen(str); strrev(str, 0, (end == 0) ? 0 : --end); }

namespace string {
namespace implementation {

template <typename T>
const T ato(const char* const str, size_type size) {
	if (!str) return T();
	T sum = 0;
	bool negative = str[0] == '-';
	size_type base = 10;
	for (auto i = 0u; i < size; ++i) {
		if ((str[i] >= 'a' && str[i] <= 'f') || (str[i] >= 'A' && str[i] <= 'F')) {
			base = 16;
		}
	}
	auto i = 0u;
	if (str[0] == '+' || negative) ++i;
	for (; i < size; ++i) {
		sum *= base;
		while (str[i] == ' ') ++i;
		switch (str[i]) {
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
				sum += str[i] - 'a' + 10;
				sum *= 16;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				sum += str[i] - '0';
				break;
			default:
				return 0;
		}
	}
	if (negative) sum *= -1;
	return sum;
}

template <typename T>
char* toa(T value, char* buffer, size_type base) {
	if (!buffer) return "";
	bool negative = value < 0 && base != 'u';
	switch (base) {
		case 'd':
		case 'u':
			base = 10;
			break;
		case 'x':
				base = 16;
				//hex = true;
			break;
		default:
			break;
	}

	// value = 2017, ['7','1','0','2','\0',0...]
	size_type i = 0;
	size_type unsigned_value = value < 0 ? static_cast<size_type>(value * -1) : static_cast<size_type>(value);
	do {
		size_type remainder = unsigned_value % base;
		buffer[i++] = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
	} while(unsigned_value /= base);

	if (negative) buffer[i++] = '-';

	buffer[i] = STREND;

	strrev(buffer, 0, i - 1);

	return buffer;
}

} // implementation
} // string

const long long atoll(const char* const str) { return string::implementation::ato<long long>(str, strlen(str)); }

const long atol(const char* const str) { return string::implementation::ato<long>(str, strlen(str)); }

const int atoi(const char* const str) { return string::implementation::ato<int>(str, strlen(str)); }

char* lltoa(long long value, char* buffer, size_type base = 10) { return string::implementation::toa(value, buffer, base); }

char* ltoa(long value, char* buffer, size_type base = 10) { return string::implementation::toa(value, buffer, base); }

char* itoa(int value, char* buffer, size_type base = 10) { return string::implementation::toa(value, buffer, base); }

} // namespace tiny os
