#ifndef SHARED_HPP
#define SHARED_HPP

#include <iostream>
#include <vector>

namespace toolbox {
template <typename T>
class shared_ptr {
 public:
  shared_ptr(T* ptr) : ptr_(ptr), count_(new int(1)) {}
  shared_ptr(const shared_ptr& other) : ptr_(other.ptr_), count_(other.count_) {
    ++(*count_);
  }
  shared_ptr& operator=(const shared_ptr& other) {
    if (this != &other) {
      if (--(*count_) == 0) {
        delete ptr_;
        delete count_;
      }
      ptr_ = other.ptr_;
      count_ = other.count_;
      ++(*count_);
    }
    return *this;
  }

  void swap(shared_ptr& other) {
    std::swap(ptr_, other.ptr_);
    std::swap(count_, other.count_);
  }

  ~shared_ptr() {
    if (*count_ == 0) {
      delete count_;
      return;
    }
    if (--(*count_) == 0) {
      delete ptr_;
      delete count_;
    }
  }

  T* get() const { return ptr_; }

  int use_count() const { return *count_; }

  void reset(T* ptr = NULL) {
    int* new_count = new int(1);
    if (ptr == NULL) {
      *new_count = 0;
    }
    if (--(*count_) == 0) {
      delete ptr_;
      delete count_;
    }
    ptr_ = ptr;
    count_ = new_count;
  }

  T& operator*() const { return *ptr_; }

  T* operator->() const { return ptr_; }

  explicit operator bool() const { return ptr_ != NULL; }

 private:
  T* ptr_;
  int* count_;
};

template <typename T>
void swap(shared_ptr<T>& lhs, shared_ptr<T>& rhs) {
  lhs.swap(rhs);
}

}  // namespace toolbox

#endif