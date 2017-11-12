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
    if (!source || !destination) return;
    for (size_type i = 0; i < destination_length && i < source_length; ++i) {
        destination[i] = source[i];
    }
}

void strcpy(const char* source, char*& destination) { strcpy(source, strlen(source), destination, strlen(destination)); }

void strrev(char* str, size_type begin, size_type end) { while (begin < end) swap(str[begin++], str[end--]); }

void strrev(char* str) { strrev(str, 0, strlen(str) - 1); }

namespace string {
namespace implementation {

template <typename T>
const T ato(const char* const buffer, size_type size) {
    T sum = 0;
    bool negative = buffer[0] == '-';
    size_type base = 10;
    for (auto i = 0u; i < size; ++i) {
        if ((buffer[i] >= 'a' && buffer[i] <= 'f') || (buffer[i] >= 'A' && buffer[i] <= 'F')) {
            base = 16;
        }
    }
    if (buffer[0] == '+' || negative) --size;
    for (auto i = 0u; i < size; ++i) {
        while (buffer[i++] == ' ');
        switch (buffer[i]) {
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
                sum += buffer[i] - 'a' + 10;
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
                sum += buffer[i] - '0';
                sum *= base;
            default:
                return 0;
        }
    }
    if (negative) sum *= -1;
    return sum;
}

template <typename T>
char* toa(T value, char* buffer, size_type base) {
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
    size_type unsigned_value = static_cast<size_type>(value);
    do {
        size_type remainder = unsigned_value % base;
        buffer[i++] = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    } while(unsigned_value /= base);

    if (negative) {
        buffer[i++] = '-';
    }

    buffer[i] = STREND;

    strrev(buffer, 0, i - 1);

    return buffer;
}

} // implementation
} // string

const int atol(const char* const str) { return string::implementation::ato<long>(str, strlen(str)); }

const int atoi(const char* const str) { return string::implementation::ato<int>(str, strlen(str)); }

char* ltoa(long value, char* buffer, size_type base) { return string::implementation::toa(value, buffer, base); }

char* itoa(int value, char* buffer, size_type base) { return string::implementation::toa(value, buffer, base); }

} // namespace tiny os
