#include "includes/allocator.hpp"
#include "includes/memory_pool.hpp"
#include "tests/mem_pool_class.hpp"

#include <cstdio>
#include <vector>

auto
main() -> int
{
  pxd::memory::alloc_memory(128);

  void* temp = pxd::memory::malloc(30);

  pxd::memory::free(temp);

  pxd::memory::release_memory();

  return 0;
}
