#include "../includes/memory_pool.hpp"

#include <algorithm>
#include <cstdint>
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
  if (memory.m_free_memories.empty()) {
    return nullptr;
  }

  if (size > memory.m_memory.size()) {
    return nullptr;
  }

  std::vector<MemoryInfo> candidates;

  for (auto& free_mem : memory.m_free_memories) {
    if (free_mem.total_size >= size) {
      candidates.push_back(free_mem);
    }
  }

  std::ranges::sort(candidates, [](const MemoryInfo& a, const MemoryInfo& b) {
    return a.total_size < b.total_size;
  });

  MemoryInfo selected = candidates[0];
  void* start_ptr = static_cast<void*>(&memory.m_memory[selected.start_index]);

  memory.m_free_memories.pop_front();

  MemoryInfo allocated  = {};
  allocated.start_index = selected.start_index;
  allocated.total_size  = size;

  memory.m_allocated_memories.push_back(allocated);

  if (selected.total_size != size) {
    MemoryInfo remaining  = {};
    remaining.start_index = selected.start_index + size;
    remaining.total_size  = selected.total_size - size;

    memory.m_free_memories.push_back(remaining);
  }

  return start_ptr;
}

void
free_memory()
{
  memory.m_memory.clear();
  memory.m_free_memories.clear();
  memory.m_allocated_memories.clear();
}

} // namespace pxd::memory
