#include "includes/memory_pool.hpp"

auto
main() -> int
{
  pxd::memory::alloc_memory(128);

  void* temp   = pxd::memory::malloc(10);
  void* temp_2 = pxd::memory::malloc(22);

  pxd::memory::free(temp_2);

  pxd::memory::release_memory();

  return 0;
}
