#pragma once

#include <cstddef>
#include <list>

namespace pxd::memory {

struct MemoryInfo;

void alloc_memory(size_t size);

auto malloc(size_t size) -> void*;
auto calloc(size_t size) -> void*;
void free(void* mem_pointer);

auto get_freed_memories() -> std::list<MemoryInfo>::iterator;
auto get_allocated_memories() -> std::list<MemoryInfo>::iterator;

void release_memory();

} // namespace pxd::memory
