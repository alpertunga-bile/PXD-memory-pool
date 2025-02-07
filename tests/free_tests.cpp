#include <gtest/gtest.h>

#include "../includes/memory_pool.hpp"

TEST(Free, Array) {
  pxd::memory::alloc_memory(128);

  void* temp = pxd::memory::malloc(30);

  pxd::memory::release_memory();
}
