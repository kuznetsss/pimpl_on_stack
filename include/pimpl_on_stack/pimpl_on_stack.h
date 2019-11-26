#pragma once

#include <type_traits>
#include <utility>

namespace pimpl_on_stack {

template <class T, std::size_t size, std::size_t align>
class Pimpl {
 public:
  template <class... Args>
  explicit Pimpl(Args... args) : ptr_(PtrFromStorage()) {
    new (ptr_) T(std::forward<Args>(args)...);
  }

  ~Pimpl() {
    static_assert(sizeof(T) == size, "Wrong size in Pimpl");
    static_assert(std::alignment_of<T>::value == align,
                  "Wrong alignment in Pimp");
    ptr_->~T();
  }

  Pimpl(Pimpl<T, size, align>&& other) noexcept { *this = std::move(other); }

  Pimpl<T, size, align>& operator=(Pimpl<T, size, align>&& other) noexcept {
    ptr_ = PtrFromStorage();
    *ptr_ = std::move(*other);
    other.ptr_ = nullptr;
  }

  Pimpl(const Pimpl<T, size, align>&) = delete;
  Pimpl<T, size, align>& operator=(const Pimpl<T, size, align>&) = delete;

  T& operator*() noexcept { return *ptr_; }
  const T& operator*() const noexcept { return *ptr_; }

  T* operator->() noexcept { return ptr_; }
  const T* operator->() const noexcept { return ptr_; }

  bool operator==(std::nullptr_t) const noexcept { return ptr_ == nullptr; }
  bool operator!=(std::nullptr_t) const noexcept { return !(*this == nullptr); }

  [[nodiscard]] bool empty() const noexcept { return ptr_ == nullptr; }

  void reset() {
    ptr_->~T();
    ptr_ = nullptr;
  }

 private:
  T* PtrFromStorage() { return reinterpret_cast<T*>(storage_); }
  std::aligned_storage_t<size, align> storage_;
  T* ptr_ = nullptr;
};

}  // namespace pimpl_on_stack