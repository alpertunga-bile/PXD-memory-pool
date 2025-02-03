#include "includes/memory_pool.hpp"

auto
main() -> int
{
  pxd::MemoryPool mem_pool(128);

  void* temp = static_cast<void*>(mem_pool.malloc(10));

  return 0;
}
