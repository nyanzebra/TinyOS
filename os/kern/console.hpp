#pragma once

#include "string.hpp"

namespace tiny_os {

#ifndef CONSOLE_CONSTANTS
#define CONSOLE_CONSTANTS

#define CONSOLE_ROWS 25u
#define CONSOLE_COLUMNS 80u

#define VGA_BUFFER (volatile short* const) 0xb8000

#define NEWLINE '\n'
#define CARRIAGE '\r'
#define TAB '\t'

#endif

namespace io {
namespace console {

enum class color_code : char {
	kBLACK      = 0,
	kBLUE       = 1,
	kGREEN      = 2,
	kCYAN       = 3,
	kRED        = 4,
	kMAGENTA    = 5,
	kBROWN      = 6,
	kLIGHTGREY  = 7,
	kDARKGREY   = 8,
	kLIGHTBLUE  = 9,
	kLIGHTGREEN = 10,
	kLIGHTCYAN  = 11,
	kLIGHTRED   = 12,
	kPINK       = 13,
	kYELLOW     = 14,
	kWHITE      = 15,
};

struct color {
	unsigned char foreground, background;
	unsigned char value;

	operator unsigned char() { return value; }

	color(const unsigned char foreground, const unsigned char background) : foreground(foreground), background(background), value(foreground | background << 4) {}
};

struct terminal_char {
	unsigned char ascii;
	struct color color;
	unsigned short value;

	operator unsigned short() { return value; }

	terminal_char(const unsigned char ascii, const struct color color) : ascii(ascii), color(color), value((short) ascii | (short) color.value << 8) {}
};

template <size_type N>
struct terminal_string {
	terminal_char value[N];
	void operator=(const char* str) {
		for (size_type i = 0; i < strlen(str) && i < N; ++i) {
			value[i] = str[i];
		}
	}
};

static volatile short* gBUFFER = VGA_BUFFER;
static char gROW = 0;
static char gCOLUMN = 0;
static color_code gFOREGROUND = color_code::kWHITE;
static color_code gBACKGROUND = color_code::kBLACK;

} // namespace console
} // namespace io
} // namespace tiny os
