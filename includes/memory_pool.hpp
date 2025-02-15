#pragma once

#include <cstddef>

namespace pxd::memory {

void
alloc_memory(size_t size);

auto
malloc(size_t size) -> void*;

auto
calloc(size_t size) -> void*;

void
free(void* mem_pointer);

void
release_memory();

auto
total_free_memory() -> size_t;
auto
total_allocated_memory() -> size_t;
auto
max_free_memory() -> size_t;
auto
min_free_memory() -> size_t;

} // namespace pxd::memory
