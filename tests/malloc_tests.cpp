#include <gtest/gtest.h>

#include "../includes/memory_pool.hpp"

#define MEMORY_POOL_DEFAULT_SIZE 128

TEST(MemoryPool, MallocEmpty)
{
  pxd::memory::alloc_memory(MEMORY_POOL_DEFAULT_SIZE);

  void* temp = pxd::memory::malloc(10);

  EXPECT_NE(temp, nullptr);

  pxd::memory::free_memory();
}

TEST(MemoryPool, MallocIntArray)
{
  pxd::memory::alloc_memory(MEMORY_POOL_DEFAULT_SIZE);

  int* arr = static_cast<int*>(pxd::memory::malloc(10));

  for (int i = 0; i < 10; ++i) {
    arr[i] = i + 1;
  }

  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(i + 1, arr[i]);
  }

  pxd::memory::free_memory();
}
