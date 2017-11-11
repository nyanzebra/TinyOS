#pragma once

#include "utility.hpp"

namespace tiny_os {

template <typename T>
class optional;

template <typename T>
class optional<T*> {
private:
  T* _ptr = nullptr;
protected:
public:
private:
protected:
public:
  optional() noexcept = default;
  optional(T* ptr) noexcept : _ptr(ptr) {}
  template <typename U>
  optional(U* ptr) noexcept : _ptr(ptr) {}
  optional(const optional&) = default;
  optional(optional&&) noexcept = default;
  ~optional() noexcept = default;

  optional& operator=(const optional&) = default;
  optional& operator=(optional&&) noexcept = default;
  optional& operator=(T* ptr) noexcept { return operator=(optional<T>(ptr)); }
  template <typename U>
  optional& operator=(U* ptr) noexcept { return operator=(optional<T>(ptr)); }

  friend bool operator==(const optional& lhs, const optional& rhs) noexcept {
    if (&lhs != &rhs) {
      return lhs._ptr == rhs._ptr;
    }
    return true;
  }

  friend bool operator==(optional&& lhs, optional&& rhs) noexcept {
    if (&lhs != &rhs) {
      return lhs._ptr == rhs._ptr;
    }
    return true;
  }

  friend bool operator!=(const optional& lhs, const optional& rhs) noexcept {
    return !(lhs == rhs);
  }

  friend bool operator!=(optional&& lhs, optional&& rhs) noexcept {
    return !(lhs == rhs);
  }

  explicit operator bool() const noexcept {
    return (_ptr);
  }

  bool has_value() const noexcept { return (_ptr); }

  const T& operator*() const noexcept { return *_ptr; }
  T& operator*() noexcept { return *_ptr; }

  const T* operator->() const noexcept { return _ptr; }
  T* operator->() noexcept { return _ptr; }

  const T& value() const noexcept { return *_ptr; }
  T& value() noexcept { return *_ptr; }

  template <typename U>
  const T& value_or(U&& other) const noexcept { return has_value() ? value() : T(forward<U>(other)); }
  template <typename U>
  T& value_or(U&& other) noexcept { return has_value() ? value() : T(forward<U>(other)); }
};

} // namespace tiny os
