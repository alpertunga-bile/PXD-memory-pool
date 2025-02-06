#include "../includes/memory_pool.hpp"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <list>
#include <vector>

namespace pxd::memory {

struct MemoryInfo
{
  size_t start_index = 0;
  size_t total_size  = 0;

  MemoryInfo()                                   = default;
  MemoryInfo(const MemoryInfo& other)            = default;
  MemoryInfo& operator=(const MemoryInfo& other) = default;
  MemoryInfo(MemoryInfo&& other)                 = default;
  MemoryInfo& operator=(MemoryInfo&& other)      = default;
  ~MemoryInfo() noexcept                         = default;

  bool operator==(const MemoryInfo& other)
  {
    return start_index == other.start_index && total_size == other.total_size;
  }

  bool operator!=(const MemoryInfo& other)
  {
    return start_index != other.start_index || total_size != other.total_size;
  }

  auto is_prev_from_given(const MemoryInfo& other) const -> bool
  {
    return (start_index + total_size) == other.start_index;
  }

  auto is_next_from_given(const MemoryInfo& other) const -> bool
  {
    return (other.start_index + other.total_size) == start_index;
  }
};

struct Memory
{
  std::vector<uint8_t>  m_memory;
  std::list<MemoryInfo> m_free_memories;
  std::list<MemoryInfo> m_allocated_memories;
};

static Memory memory;

void
alloc_memory(size_t size)
{
  memory.m_memory.resize(size);

  MemoryInfo all  = {};
  all.start_index = 0;
  all.total_size  = size;

  memory.m_free_memories.push_back(all);
}

auto
malloc(size_t size) -> void*
{
  if (memory.m_free_memories.empty() || size > memory.m_memory.size()) {
    return nullptr;
  }

  if (memory.m_free_memories.size() > 1) {
    memory.m_free_memories.sort(
      [](const MemoryInfo& lhs, const MemoryInfo& rhs) {
        return lhs.total_size < rhs.total_size;
      });
  }

  auto selected = memory.m_free_memories.end();

  for (auto iter = memory.m_free_memories.begin();
       iter != memory.m_free_memories.end();
       ++iter) {
    if (iter->total_size >= size) {
      selected = iter;
      break;
    }
  }

  if (selected == memory.m_free_memories.end()) {
    return nullptr;
  }

  MemoryInfo allocated  = {};
  allocated.start_index = selected->start_index;
  allocated.total_size  = size;

  memory.m_allocated_memories.push_back(allocated);

  if (selected->total_size > size) {
    selected->start_index += size;
    selected->total_size  -= size;
  } else {
    auto num_removed = memory.m_free_memories.remove(*selected);
  }

  void* start_ptr = static_cast<void*>(&memory.m_memory[selected->start_index]);

  return start_ptr;
}

auto
calloc(size_t size) -> void*
{
  void* result = malloc(size);

  if (nullptr == result) {
    return nullptr;
  }

  std::memset(result, 0, size);

  return result;
}

void
merge_with_given_memory(MemoryInfo found_info)
{
  size_t num_removed = 0;

  MemoryInfo prev_of_found = {};
  MemoryInfo next_of_found = {};

  for (auto& free_mem : memory.m_free_memories) {
    if (found_info.is_prev_from_given(free_mem)) {
      next_of_found = free_mem;
      break;
    }

    if (found_info.is_next_from_given(free_mem)) {
      prev_of_found = free_mem;
      break;
    }
  }

  if (next_of_found.total_size > 0) {
    num_removed = memory.m_free_memories.remove(next_of_found);

    found_info.total_size += next_of_found.total_size;

    memory.m_free_memories.push_back(found_info);
  } else if (prev_of_found.total_size > 0) {
    num_removed = memory.m_free_memories.remove(prev_of_found);

    prev_of_found.total_size += found_info.total_size;

    memory.m_free_memories.push_back(prev_of_found);
  } else {
    memory.m_free_memories.push_back(found_info);
  }
}

void
free(void* mem_pointer)
{
  if (memory.m_allocated_memories.empty()) {
    return;
  }

  MemoryInfo found_info   = {};
  size_t     alloc_length = memory.m_allocated_memories.size();

  for (auto& alloc_mem : memory.m_allocated_memories) {
    void* ptr = static_cast<void*>(&memory.m_memory[alloc_mem.start_index]);

    if (ptr == mem_pointer) {
      found_info = alloc_mem;
      break;
    }
  }

  if (found_info.total_size == 0) {
    return;
  }

  auto num_removed = memory.m_allocated_memories.remove(found_info);

  merge_with_given_memory(found_info);
}

void
release_memory()
{
  memory.m_memory.clear();
  memory.m_free_memories.clear();
  memory.m_allocated_memories.clear();
}

} // namespace pxd::memory
