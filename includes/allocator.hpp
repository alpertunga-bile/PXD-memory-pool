#pragma once

#include "memory_pool.hpp"
#include <cstddef>
#include <limits>
#include <new>
#include <type_traits>

namespace pxd::memory {

template<class T>
struct allocator
{
  using value_type                             = T;
  using pointer                                = T*;
  using const_pointer                          = const T*;
  using reference                              = T&;
  using const_reference                        = const T&;
  using size_type                              = std::size_t;
  using difference_type                        = std::ptrdiff_t;
  using propagate_on_container_move_assignment = std::true_type;

  allocator()                                  = default;
  allocator(const allocator& other)            = default;
  allocator& operator=(const allocator& other) = default;
  allocator(allocator&& other)                 = default;
  allocator& operator=(allocator&& other)      = default;
  ~allocator() noexcept                        = default;

  constexpr auto allocate(size_type n) -> T*
  {
    if ((std::numeric_limits<size_type>::max() / sizeof(value_type)) < n) {
      throw std::bad_array_new_length();
    }

    void* ptr = pxd::memory::malloc(n * sizeof(value_type));

    if (nullptr == ptr) {
      throw std::bad_alloc();
    }

    return static_cast<pointer>(ptr);
  }

  constexpr void deallocate(T* p, size_type n) { pxd::memory::free(p); }
};

} // namespace pxd::memory
