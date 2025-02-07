#include <gtest/gtest.h>

#include "../includes/memory_pool.hpp"

TEST(Free, Array)
{
  pxd::memory::alloc_memory(128);

  void* temp = pxd::memory::malloc(30);

  pxd::memory::release_memory();
}

TEST(Free, PrevMemory)
{
  pxd::memory::alloc_memory(128);

  void* temp   = pxd::memory::malloc(10);
  void* temp_2 = pxd::memory::malloc(10);
  void* temp_3 = pxd::memory::malloc(10);

  pxd::memory::free(temp_3);

  pxd::memory::release_memory();
}

TEST(Free, NextMemory)
{
  pxd::memory::alloc_memory(128);

  void* temp   = pxd::memory::malloc(10);
  void* temp_2 = pxd::memory::malloc(10);
  void* temp_3 = pxd::memory::malloc(10);

  pxd::memory::free(temp_2);
  pxd::memory::free(temp);

  pxd::memory::release_memory();
}

TEST(Free, MiddleMemory)
{
  pxd::memory::alloc_memory(128);

  void* temp   = pxd::memory::malloc(10);
  void* temp_2 = pxd::memory::malloc(10);
  void* temp_3 = pxd::memory::malloc(10);

  pxd::memory::free(temp);
  pxd::memory::free(temp_3);
  pxd::memory::free(temp_2);

  pxd::memory::release_memory();
}
