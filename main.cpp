#include "includes/allocator.hpp"
#include "includes/memory_pool.hpp"
#include "tests/mem_pool_class.hpp"

#include <stdio.h>
#include <vector>

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

  pxd::memory::alloc_memory(128);

  void* temp   = pxd::memory::malloc(10);
  void* temp_2 = pxd::memory::malloc(10);
  void* temp_3 = pxd::memory::malloc(10);

  pxd::memory::free(temp_2);
  pxd::memory::free(temp);
  pxd::memory::free(temp_3);

  auto size = pxd::memory::total_free_memory();

  pxd::memory::release_memory();

  */

  pxd::memory::alloc_memory(1024);

  {
    std::vector<int, pxd::memory::allocator<int>> temp(50);

    printf("-------------------------------------\n");
    printf("Total allocated: %zu\n", pxd::memory::total_allocated_memory());
    printf("Total free     : %zu\n", pxd::memory::total_free_memory());
    printf("Max free       : %zu\n", pxd::memory::max_free_memory());
    printf("-------------------------------------\n");

    temp.resize(10);

    printf("-------------------------------------\n");
    printf("Total allocated: %zu\n", pxd::memory::total_allocated_memory());
    printf("Total free     : %zu\n", pxd::memory::total_free_memory());
    printf("Max free       : %zu\n", pxd::memory::max_free_memory());
    printf("-------------------------------------\n");
  }

  pxd::memory::release_memory();

  return 0;
}
