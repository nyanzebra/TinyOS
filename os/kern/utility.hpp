#pragma once

namespace tiny_os {

typedef unsigned long long size_type;

#ifndef assert
void _assert(const char* const expr, const char* const message, const char* const file, const char* const function, size_type line) {
  //TODO: we need to have an exception file for printing exceptions and killing the kernel
}
#define assert(expr, message) (if (!expr) _assert(#expr, message, __FILE__, __FUNCTION__, __LINE__))
#endif

#ifndef BIT_FUNCTIONS
#define BIT_FUNCTIONS
#define IS_BIT_SET(x, bit) x & (1 << bit)
#endif

#ifndef __BLOCK
#define __BLOCK while (true);
#define __BLOCK_IF_TRUE(x) while (x);
#endif

#ifndef offsetof
#define offsetof(type, member) ((size_type) & ((type*)0)->member)
#endif

template <typename T, typename U>
void memcpy(const T* const source, size_type source_size, U*& destination, size_type destination_size) {
    for (auto i = 0u; i < source_size && i < destination_size; ++i) {
        destination[i] = source[i];
    }
}

template <typename T, typename U>
void memcpy(const T* const source, U*& destination) { memcpy(source, sizeof(source) / sizeof(T), destination, sizeof(destination) / sizeof(U)); }

template <typename T, typename U = T>
void swap(T& a, U& b) {
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
struct remove_reference { typedef T type; };

template <typename T>
struct remove_reference<T&> { typedef T type; };

template <typename T>
struct remove_reference<T&&> { typedef T type; };

template <typename T>
using remove_reference_type = typename remove_reference<T>::type;

template <typename T>
constexpr remove_reference_type<T>&& move(T&& arg) { return static_cast<remove_reference_type<T>&&>(arg); }

template <typename InputIterator, typename OutputIterator>
OutputIterator move(InputIterator first, InputIterator last, OutputIterator destination) {
	while (first != last) *destination++ = move(*first++);
	return destination;
}

template <typename T, T v>
struct integral_constant {
	typedef T value_type;
	typedef const T const_value_type;

	typedef T& reference_type;
	typedef const T& const_reference_type;

	typedef T&& universal_type;

	typedef T* pointer_type;
	typedef const T* const_pointer_type;
	typedef T* const pointer_const_type;
	typedef const T* const const_pointer_const_type;

	static constexpr value_type value = v;

	typedef integral_constant type;
	constexpr operator value_type() const noexcept { return value; }
	constexpr value_type operator()() const noexcept { return value; }
};

template <bool B>
using bool_constant = integral_constant<bool, B>;

using true_type = bool_constant<true>;

using false_type = bool_constant<false>;

template <typename T>
struct is_lvalue_reference : public false_type {};

template <typename T>
struct is_lvalue_reference<T&> : public true_type {};

template <typename T>
constexpr bool is_lvalue_reference_value = is_lvalue_reference<T>::value;

template <typename T>
constexpr T&& forward(remove_reference_type<T>&& arg) {
	static_assert(!is_lvalue_reference_value<T>, "Cannot forward an rvalue as an lvalue");
	return static_cast<remove_reference_type<T>&&>(arg);
}

template <typename T>
constexpr T&& forward(remove_reference_type<T>& arg) { return static_cast<remove_reference_type<T>&&>(arg); }

template <typename InputIterator, typename OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator destination) {
	while (first != last) *destination++ = *first++;
	return destination;
}

} // namespace tiny os
