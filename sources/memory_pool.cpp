#include "../includes/memory_pool.hpp"

#include <algorithm>

namespace pxd {

MemoryPool::MemoryPool(size_t size)
{
  m_memory.resize(size);

  MemoryInfo all  = {};
  all.start_index = 0;
  all.total_size  = size;

  m_free_memories.push_back(all);
}

auto
MemoryPool::malloc(size_t size) -> void*
{
  if (m_free_memories.empty()) {
    return nullptr;
  }

  if (size > m_memory.size()) {
    return nullptr;
  }

  std::vector<MemoryInfo> candidates;

  for (auto& m_free_memorie : m_free_memories) {
    if (m_free_memorie.total_size >= size) {
      candidates.push_back(m_free_memorie);
    }
  }

  std::ranges::sort(candidates, [](const MemoryInfo& a, const MemoryInfo& b) {
    return a.total_size < b.total_size;
  });

  MemoryInfo selected  = candidates[0];
  void*      start_ptr = static_cast<void*>(&m_memory[selected.start_index]);

  m_free_memories.pop_front();

  MemoryInfo allocated  = {};
  allocated.start_index = selected.start_index;
  allocated.total_size  = size;

  m_allocated_memories.push_back(allocated);

  if (selected.total_size != size) {
    MemoryInfo remaining  = {};
    remaining.start_index = selected.start_index + size;
    remaining.total_size  = selected.total_size - size;

    m_free_memories.push_back(remaining);
  }

  return start_ptr;
}

} // namespace pxd
