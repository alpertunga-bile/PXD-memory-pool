#pragma once

#include <cstddef>

namespace pxd::memory {

constexpr size_t SIZE_1KB = 1024;
constexpr size_t SIZE_1MB = static_cast<size_t>(1024) * 1024;
constexpr size_t SIZE_1GB = static_cast<size_t>(1024) * 1024 * 1024;

void
alloc_memory(size_t size);

[[nodiscard]] auto
malloc(size_t size) noexcept -> void*;

[[nodiscard]] auto
calloc(size_t size) noexcept -> void*;

void
free(void* mem_pointer) noexcept;

void
release_memory() noexcept;

auto
total_free_memory() -> size_t;
auto
total_allocated_memory() -> size_t;
auto
max_free_memory() -> size_t;
auto
min_free_memory() -> size_t;

} // namespace pxd::memory
