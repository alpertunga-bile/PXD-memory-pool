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

TEST(MemoryPool, MallocFloatArray)
{
  pxd::memory::alloc_memory(MEMORY_POOL_DEFAULT_SIZE);

  float* arr = static_cast<float*>(pxd::memory::malloc(10 * sizeof(float)));

  for (int i = 0; i < 10; ++i) {
    arr[i] = static_cast<float>(i + 1);
  }

  for (int i = 0; i < 10; ++i) {
    EXPECT_FLOAT_EQ(static_cast<float>(i + 1), arr[i]);
  }

  pxd::memory::free_memory();
}

TEST(MemoryPool, MallocDoubleArray)
{
  pxd::memory::alloc_memory(MEMORY_POOL_DEFAULT_SIZE);

  double* arr = static_cast<double*>(pxd::memory::malloc(10 * sizeof(double)));

  for (int i = 0; i < 10; ++i) {
    arr[i] = static_cast<double>(i + 1);
  }

  for (int i = 0; i < 10; ++i) {
    EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), arr[i]);
  }

  pxd::memory::free_memory();
}

struct Temp
{
  int    m_int;
  float  m_float;
  double m_double;
};

TEST(MemoryPool, MallocStruct)
{
  pxd::memory::alloc_memory(MEMORY_POOL_DEFAULT_SIZE);

  Temp res     = {};
  res.m_int    = 12312;
  res.m_float  = 12.432423f;
  res.m_double = 23123123.434541245;

  Temp* temp = static_cast<Temp*>(pxd::memory::malloc(sizeof(Temp)));

  temp->m_int    = res.m_int;
  temp->m_float  = res.m_float;
  temp->m_double = res.m_double;

  EXPECT_EQ(temp->m_int, res.m_int);
  EXPECT_FLOAT_EQ(temp->m_float, res.m_float);
  EXPECT_DOUBLE_EQ(temp->m_double, res.m_double);

  pxd::memory::free_memory();
}
