#pragma once

#include <type_traits>
#include <utility>

namespace pimpl_on_stack {

template <class T, std::size_t Size, std::size_t Alignment>
class Pimpl {
 public:
  template <class... Args>
  explicit Pimpl(Args... args) : ptr_(PtrFromStorage()) {
    new (ptr_) T(std::forward<Args>(args)...);
  }

  ~Pimpl() {
    CheckSizeAndAlignment<Size, Alignment>();
    ptr_->~T();
  }

  Pimpl(Pimpl<T, Size, Alignment>&& other) noexcept {
    *this = std::move(other);
  }

  Pimpl<T, Size, Alignment>& operator=(
      Pimpl<T, Size, Alignment>&& other) noexcept {
    ptr_ = PtrFromStorage();
    *ptr_ = std::move(*other);
    other.ptr_ = nullptr;
  }

  Pimpl(const Pimpl<T, Size, Alignment>&) = delete;
  Pimpl<T, Size, Alignment>& operator=(const Pimpl<T, Size, Alignment>&) =
      delete;

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
  T* PtrFromStorage() { return reinterpret_cast<T*>(&storage_); }
  template <std::size_t UsedSize, std::size_t UsedAlignment,
            std::size_t RealSize = sizeof(T),
            std::size_t RealAlignment = std::alignment_of<T>::value>
  static constexpr void CheckSizeAndAlignment() {
    static_assert(UsedSize == RealSize, "Wrong size used in pimpl.");
    static_assert(UsedAlignment == RealAlignment,
                  "Wrong alignment used in pimpl.");
  }
  std::aligned_storage_t<Size, Alignment> storage_;
  T* ptr_ = nullptr;
};

}  // namespace pimpl_on_stack