#pragma once

#include "string.hpp"
#include "console.hpp"

namespace tiny_os {
namespace io {
namespace console {

inline void foreground(const color_code color) { gFOREGROUND = color; }
inline void background(const color_code color) { gBACKGROUND = color; }
inline const color_code foreground() { return gFOREGROUND; }
inline const color_code background() { return gBACKGROUND; }

inline void debug_colors() { gFOREGROUND = color_code::kGREEN; gBACKGROUND = color_code::kBLACK; }
inline void error_colors() { gFOREGROUND = color_code::kRED; gBACKGROUND = color_code::kBLACK; }
inline void default_colors() { gFOREGROUND = color_code::kWHITE; gBACKGROUND = color_code::kBLACK; }

inline void clear() {
	for (size_type i = 0; i < (CONSOLE_ROWS * CONSOLE_COLUMNS); ++i) {
		gBUFFER[i] = 0;
	}
	gROW = 0;
	gCOLUMN = 0;
}

inline void putchar(const char* str) {}

inline void putchar(const char ascii) {
	if (ascii != STREND) {
		if (gCOLUMN == CONSOLE_COLUMNS) {
			gROW++;
			gCOLUMN = 0;
			if (gROW == CONSOLE_ROWS) {
				//_shift_up(); TODO
			}
		}
		if (ascii == NEWLINE) {
			gCOLUMN = 0;
			gROW++;
		} else {
			const char color = (char) foreground() | (char) background() << 4;
			gBUFFER[(gROW * CONSOLE_COLUMNS) + gCOLUMN++] = (short) ascii | (short) color << 8;
		}
	}
}

namespace implementation {

void printf(const char* const str, const size_type loc, const size_type size) { for (size_type i = loc; i < size; ++i) putchar(str[i]); }

void printf(const char* const str) { printf(str, 0u, strlen(str)); }

void __log(const char* const message, const char* const info, const char* const func, const char* const level) { printf(level); printf(message); printf(info); printf(func); putchar(STREND); }

const char format_as_char(const char* value, char c) { return value[0]; }

const char format_as_char(char value, char c) { return value; }

const char* format_as_string(char* value, char c) { return value ? value : "(null)"; }

const char* format_as_string(const char* value, char c) { return value ? value : "(null)"; }

const char* format_as_string(long value, char c) {
	char buffer[20];
	return ltoa(value, buffer, c);
}

template <typename T, typename... Rest>
void printf(const char* str, const size_type loc, const size_type size, T value, Rest... rest) {
	const char* ptr;
	for (size_type i = loc; i < size; ++i) {
		char c = str[i];
		if (c == '%') {
			char type = str[i + 1];
			switch (type) {
				case 'u':
				case 'd':
				case 'x':
					char buffer[20];
					ptr = ltoa((long long)value, buffer, type);
					printf(ptr);
					break;
				case 'c':
					putchar(value);
					break;
				case 's':
					ptr = reinterpret_cast<const char*>(value) ? reinterpret_cast<const char*>(value) : "(null)";
					printf(ptr);
					break;
				default:
					putchar(value);
					break;
			}
			printf(str, i + 2, size, rest...);
			break;
		} else {
			putchar(c);
		}
	}
}

} // namespace implementation

template <typename... T>
void printf(const char* str, T... values) { implementation::printf(str, 0, strlen(str), values...); }

void log(const char* message, const char* info, const char* func, const char* level) { implementation::__log(message, info, func, level); }

#ifndef IDENTITY
#define __IDENTITY(x) x
#define IDENTITY(x) __IDENTITY(x)
#endif

#ifndef CONCATENATE
#define __CONCATENATE(a, b) a ## b
#define CONCATENATE(a, b) __CONCATENATE(a, b)
#endif

#ifndef STRING_CONCATENATE
#define __STRING_CONCATENATE(a, b) a b
#define STRING_CONCATENATE(a, b) __STRING_CONCATENATE(a, b)
#endif

#ifndef STRINGIFY
#define __STRINGIFY(a) #a
#define STRINGIFY(a) __STRINGIFY(a)
#endif

#ifndef FILE_FUNCTION_LINE
#define __FILE_INFO(file) STRING_CONCATENATE(" file: ", file)
#define FILE_INFO() __FILE_INFO(__FILE__)
#define __FUNCTION_INFO() STRING_CONCATENATE(" function: ", "")
#define FUNCTION_INFO() __FUNCTION_INFO()
#define __LINE_INFO(line) STRING_CONCATENATE(" line: ", STRINGIFY(line))
#define LINE_INFO() __LINE_INFO(__LINE__)
#define FILE_FUNCTION_LINE() STRING_CONCATENATE(STRING_CONCATENATE(FILE_INFO(), LINE_INFO()), FUNCTION_INFO())
#endif

#ifndef PRINT
#define __PRINT(str, args...) io::console::printf(str, args);
#define PRINT(str, args...) __PRINT(str, args)
#endif

#ifndef LOG
#define __LOG(str, func, level) io::console::log(str, FILE_FUNCTION_LINE(), func, level)
#define LOG(str, func, level) __LOG(str, func, level)
	#ifndef DEBUG
	#define __DEBUG(str, func) LOG(str, func, "[debug] ")
	#define DEBUG(str) io::console::debug_colors(); __DEBUG(str, __PRETTY_FUNCTION__); io::console::default_colors();
	#endif
	#ifndef ERROR
	#define __ERROR(str, func) LOG(str, func, "[error] ")
	#define ERROR(str) io::console::error_colors(); __ERROR(str, __PRETTY_FUNCTION__); io::console::default_colors();
	#endif
#endif

#ifndef BLOCK_IF_TRUE_PRINT
#define _BLOCK_IF_TRUE_PRINT(expr, message) if (expr) { io::console::printf(message); } __BLOCK_IF_TRUE(expr);
#define BLOCK_IF_TRUE_PRINT(expr, message) _BLOCK_IF_TRUE_PRINT(expr, message)
#endif

} // namespace console
} // namespace io
} // namespace tiny os
