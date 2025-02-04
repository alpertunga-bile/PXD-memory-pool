#include "includes/memory_pool.hpp"

auto
main() -> int
{
  pxd::memory::alloc_memory(128);

  void* temp = pxd::memory::malloc(10);

  pxd::memory::free_memory();

  return 0;
}
