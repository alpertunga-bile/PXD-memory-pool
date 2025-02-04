#include <gtest/gtest.h>

#include "../includes/memory_pool.hpp"

TEST(MemoryPool, CAllocTest) {
  pxd::memory::alloc_memory(128);

  int* temp_arr = static_cast<int*>(pxd::memory::calloc(10));

  for(int i = 0; i < 10; ++i) {
    EXPECT_EQ(temp_arr[i], 0);
  }

  pxd::memory::free_memory();
}
