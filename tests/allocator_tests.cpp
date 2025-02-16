#include <gtest/gtest.h>

#include "../includes/allocator.hpp"
#include "../includes/memory_pool.hpp"

#include <vector>

TEST(Allocator, Vector)
{
  pxd::memory::alloc_memory(1024);

  EXPECT_EQ(1024, pxd::memory::total_free_memory());
  EXPECT_EQ(1024, pxd::memory::max_free_memory());

  std::vector<int, pxd::memory::allocator<int>> temp_vec(50);

  EXPECT_EQ(824, pxd::memory::total_free_memory());
  EXPECT_EQ(824, pxd::memory::max_free_memory());
  EXPECT_EQ(200, pxd::memory::total_allocated_memory());

  pxd::memory::release_memory();
}

TEST(Allocator, ResizeVector)
{
  pxd::memory::alloc_memory(1024);

  EXPECT_EQ(1024, pxd::memory::total_free_memory());
  EXPECT_EQ(1024, pxd::memory::max_free_memory());

  std::vector<int, pxd::memory::allocator<int>> temp_vec(50);

  EXPECT_EQ(824, pxd::memory::total_free_memory());
  EXPECT_EQ(824, pxd::memory::max_free_memory());
  EXPECT_EQ(200, pxd::memory::total_allocated_memory());

  temp_vec.resize(10);

  EXPECT_EQ(824, pxd::memory::total_free_memory());
  EXPECT_EQ(824, pxd::memory::max_free_memory());
  EXPECT_EQ(200, pxd::memory::total_allocated_memory());
  EXPECT_EQ(10, temp_vec.size());

  pxd::memory::release_memory();
}

TEST(Allocator, MultipleVectors)
{
  pxd::memory::alloc_memory(1024);

  EXPECT_EQ(1024, pxd::memory::total_free_memory());
  EXPECT_EQ(1024, pxd::memory::max_free_memory());

  std::vector<int, pxd::memory::allocator<int>> temp_vec(50);

  EXPECT_EQ(824, pxd::memory::total_free_memory());
  EXPECT_EQ(824, pxd::memory::max_free_memory());
  EXPECT_EQ(200, pxd::memory::total_allocated_memory());

  std::vector<int, pxd::memory::allocator<int>> temp_vec2(50);

  EXPECT_EQ(624, pxd::memory::total_free_memory());
  EXPECT_EQ(624, pxd::memory::max_free_memory());
  EXPECT_EQ(400, pxd::memory::total_allocated_memory());

  pxd::memory::release_memory();
}
