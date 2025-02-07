#include <gtest/gtest.h>

#include "../includes/memory_pool.hpp"

TEST(Calloc, Array) {
  pxd::memory::alloc_memory(128);

  int* temp_arr = static_cast<int*>(pxd::memory::calloc(10));

  for(int i = 0; i < 10; ++i) {
    EXPECT_EQ(temp_arr[i], 0);
  }

  pxd::memory::release_memory();
}

TEST(Calloc, MoreSize) {
  pxd::memory::alloc_memory(128);

  void* temp = pxd::memory::malloc(256);

  EXPECT_EQ(temp, nullptr);

  pxd::memory::release_memory();
}
