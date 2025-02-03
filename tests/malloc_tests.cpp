#include <gtest/gtest.h>

#include "../includes/memory_pool.hpp"

#define MEMORY_POOL_DEFAULT_SIZE 128

TEST(MemoryPool, MallocEmpty)
{
  pxd::MemoryPool memory_pool(MEMORY_POOL_DEFAULT_SIZE);

  void* temp = memory_pool.malloc(10);

  EXPECT_NE(temp, nullptr);
}

TEST(MemoryPool, MallocIntArray)
{
  pxd::MemoryPool memory_pool(MEMORY_POOL_DEFAULT_SIZE);

  int* arr = static_cast<int*>(memory_pool.malloc(10));

  for (int i = 0; i < 10; ++i) {
    arr[i] = i + 1;
  }

  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(i + 1, arr[i]);
  }
}
