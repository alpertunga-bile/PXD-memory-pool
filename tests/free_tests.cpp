#include <gtest/gtest.h>

#include "../includes/memory_pool.hpp"

TEST(Free, Array)
{
  pxd::memory::alloc_memory(128);

  void* temp = pxd::memory::malloc(30);

  EXPECT_EQ(98, pxd::memory::total_free_memory());

  pxd::memory::free(temp);

  EXPECT_EQ(128, pxd::memory::total_free_memory());

  pxd::memory::release_memory();
}

TEST(Free, PrevMemory)
{
  pxd::memory::alloc_memory(128);

  void* temp   = pxd::memory::malloc(10);
  void* temp_2 = pxd::memory::malloc(10);
  void* temp_3 = pxd::memory::malloc(10);

  EXPECT_EQ(98, pxd::memory::total_free_memory());

  pxd::memory::free(temp_3);

  EXPECT_EQ(108, pxd::memory::total_free_memory());
  EXPECT_EQ(108, pxd::memory::max_free_memory());

  pxd::memory::release_memory();
}

TEST(Free, NextMemory)
{
  pxd::memory::alloc_memory(128);

  void* temp   = pxd::memory::malloc(10);
  void* temp_2 = pxd::memory::malloc(10);
  void* temp_3 = pxd::memory::malloc(10);

  EXPECT_EQ(98, pxd::memory::total_free_memory());

  pxd::memory::free(temp_2);

  EXPECT_EQ(108, pxd::memory::total_free_memory());
  EXPECT_EQ(98, pxd::memory::max_free_memory());

  pxd::memory::free(temp);

  EXPECT_EQ(118, pxd::memory::total_free_memory());
  EXPECT_EQ(20, pxd::memory::min_free_memory());

  pxd::memory::release_memory();
}

TEST(Free, NextMemory2)
{
  pxd::memory::alloc_memory(128);

  void* temp   = pxd::memory::malloc(10);
  void* temp_2 = pxd::memory::malloc(10);
  void* temp_3 = pxd::memory::malloc(10);
  void* temp_4 = pxd::memory::malloc(10);
  void* temp_5 = pxd::memory::malloc(10);

  EXPECT_EQ(78, pxd::memory::total_free_memory());

  pxd::memory::free(temp_2);

  EXPECT_EQ(88, pxd::memory::total_free_memory());
  EXPECT_EQ(10, pxd::memory::min_free_memory());

  pxd::memory::free(temp_3);

  EXPECT_EQ(98, pxd::memory::total_free_memory());
  EXPECT_EQ(20, pxd::memory::min_free_memory());

  pxd::memory::release_memory();
}

TEST(Free, MiddleMemory)
{
  pxd::memory::alloc_memory(128);

  void* temp   = pxd::memory::malloc(10);
  void* temp_2 = pxd::memory::malloc(10);
  void* temp_3 = pxd::memory::malloc(10);

  EXPECT_EQ(98, pxd::memory::total_free_memory());

  pxd::memory::free(temp);

  EXPECT_EQ(108, pxd::memory::total_free_memory());
  EXPECT_EQ(10, pxd::memory::min_free_memory());

  pxd::memory::free(temp_3);

  EXPECT_EQ(118, pxd::memory::total_free_memory());
  EXPECT_EQ(10, pxd::memory::min_free_memory());

  pxd::memory::free(temp_2);

  EXPECT_EQ(128, pxd::memory::total_free_memory());

  pxd::memory::release_memory();
}

TEST(Free, MiddleMemory2)
{
  pxd::memory::alloc_memory(128);

  void* temp   = pxd::memory::malloc(10);
  void* temp_2 = pxd::memory::malloc(10);
  void* temp_3 = pxd::memory::malloc(10);
  void* temp_4 = pxd::memory::malloc(10);
  void* temp_5 = pxd::memory::malloc(10);

  EXPECT_EQ(78, pxd::memory::total_free_memory());

  pxd::memory::free(temp_2);

  EXPECT_EQ(88, pxd::memory::total_free_memory());
  EXPECT_EQ(10, pxd::memory::min_free_memory());

  pxd::memory::free(temp_4);

  EXPECT_EQ(98, pxd::memory::total_free_memory());
  EXPECT_EQ(10, pxd::memory::min_free_memory());

  pxd::memory::free(temp_3);

  EXPECT_EQ(108, pxd::memory::total_free_memory());
  EXPECT_EQ(30, pxd::memory::min_free_memory());

  pxd::memory::release_memory();
}
