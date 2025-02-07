#pragma once

#include <cstddef>

namespace pxd::memory {

void alloc_memory(size_t size);

auto malloc(size_t size) -> void*;
auto calloc(size_t size) -> void*;
void free(void* mem_pointer);

void print_allocated_memories();
void print_freed_memories();

void release_memory();

} // namespace pxd::memory
