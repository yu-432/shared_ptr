#ifndef SHARED_HPP
#define SHARED_HPP

#include <iostream>
#include <vector>

namespace toolbox {
template <typename T>
class shared_ptr {
 public:
  shared_ptr() : _ptr(NULL), _count(new int(0)) {}

  explicit shared_ptr(T* ptr) : _ptr(ptr), _count(new int(1)) {
    if (ptr == NULL) {
      *_count = 0;
    }
  }

  shared_ptr(const shared_ptr& other) : _ptr(other._ptr), _count(other._count) {
    if (_ptr != NULL) {
      ++(*_count);
    } else {
      _count = new int(0);
    }
  }

  shared_ptr& operator=(const shared_ptr& other) {
    if (this != &other) {
      if (--(*_count) <= 0) {
        delete _ptr;
        delete _count;
      }
      _ptr = other._ptr;
      _count = other._count;
      if (_ptr != NULL) {
        ++(*_count);
      } else {
        _count = new int(0);
      }
    }
    return *this;
  }

  void swap(shared_ptr& other) {
    std::swap(_ptr, other._ptr);
    std::swap(_count, other._count);
  }

  ~shared_ptr() {
    if (*_count == 0) {
      delete _count;
      return;
    }
    if (--(*_count) == 0) {
      delete _ptr;
      delete _count;
    }
  }

  T* const get() const { return _ptr; }

  int use_count() const { return *_count; }

  void reset(T* ptr = NULL) {
    int* new_count = new int(1);
    if (ptr == NULL) {
      *new_count = 0;
    }
    if (--(*_count) == 0) {
      delete _ptr;
      delete _count;
    }
    _ptr = ptr;
    _count = new_count;
  }

  T& operator*() const { return *_ptr; }

  T* operator->() const { return _ptr; }

  explicit operator bool() const { return _ptr != NULL; }

 private:
  T* _ptr;
  int* _count;
};

template <typename T>
void swap(shared_ptr<T>& lhs, shared_ptr<T>& rhs) {
  lhs.swap(rhs);
}
}  // namespace toolbox

#endif