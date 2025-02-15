#include "tests/mem_pool_class.hpp"
#include "includes/memory_pool.hpp"

auto
main() -> int
{
  /*
  pxd::memory::MemoryPool memory(128);

  auto* temp   = memory.malloc<void>(10);
  auto* temp_2 = memory.malloc<void>(10);
  auto* temp_3 = memory.malloc<void>(10);

  memory.free(temp_3);
  memory.free(temp);
  memory.free(temp_2);

  */

  pxd::memory::alloc_memory(128);

  void* temp   = pxd::memory::malloc(10);
  void* temp_2 = pxd::memory::malloc(10);
  void* temp_3 = pxd::memory::malloc(10);

  pxd::memory::free(temp_2);
  pxd::memory::free(temp);
  pxd::memory::free(temp_3);

  auto size = pxd::memory::total_free_memory();

  pxd::memory::release_memory();

  return 0;
}
