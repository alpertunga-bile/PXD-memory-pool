#include "../includes/memory_pool.hpp"

#include <algorithm>
#include <cstdint>
#include <cstring>
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

  [[nodiscard]] auto empty() const -> bool { return total_size == 0; }

  [[nodiscard]] auto is_prev_from_given(const MemoryInfo& other) const -> bool
  {
    return (start_index + total_size) == other.start_index;
  }

  [[nodiscard]] auto is_next_from_given(const MemoryInfo& other) const -> bool
  {
    return (other.start_index + other.total_size) == start_index;
  }
};

constexpr bool
operator==(const MemoryInfo& lhs, const MemoryInfo& rhs)
{
  return lhs.start_index == rhs.start_index && lhs.total_size == rhs.total_size;
}

constexpr bool
operator!=(const MemoryInfo& lhs, const MemoryInfo& rhs)
{
  return lhs.start_index != rhs.start_index || lhs.total_size != rhs.total_size;
}

struct Memory
{
  std::vector<uint8_t>    m_memory;
  std::vector<MemoryInfo> m_freed;
  std::vector<MemoryInfo> m_allocated;
};

static Memory memory;

void
alloc_memory(size_t size)
{
  memory.m_memory.resize(size);

  MemoryInfo all  = {};
  all.start_index = 0;
  all.total_size  = size;

  memory.m_freed.push_back(all);
}

auto
malloc(size_t size) -> void*
{
  if (memory.m_freed.empty() || size > memory.m_memory.size()) {
    return nullptr;
  }

  if (memory.m_freed.size() > 1) {
    std::sort(memory.m_freed.begin(),
              memory.m_freed.end(),
              [](const MemoryInfo& lhs, const MemoryInfo& rhs) {
                return lhs.total_size < rhs.total_size;
              });
  }

  auto selected = memory.m_freed.end();

  for (auto iter = memory.m_freed.begin(); iter != memory.m_freed.end();
       ++iter) {
    if (iter->total_size >= size) {
      selected = iter;
      break;
    }
  }

  if (selected == memory.m_freed.end()) {
    return nullptr;
  }

  void* start_ptr = static_cast<void*>(&memory.m_memory[selected->start_index]);

  MemoryInfo allocated  = {};
  allocated.start_index = selected->start_index;
  allocated.total_size  = size;

  memory.m_allocated.push_back(allocated);

  if (selected->total_size > size) {
    selected->start_index += size;
    selected->total_size  -= size;
  } else {
    memory.m_freed.erase(selected);
  }

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
free(void* mem_pointer)
{
  /*
   * there are no allocations
   */
  if (memory.m_allocated.empty()) {
    return;
  }

  auto found_info =
    std::find_if(memory.m_allocated.begin(),
                 memory.m_allocated.end(),
                 [&mem_pointer](const MemoryInfo& info) {
                   return mem_pointer == static_cast<void*>(
                                           &memory.m_memory[info.start_index]);
                 });

  if (found_info == memory.m_allocated.end()) {
    return;
  }

  std::vector<MemoryInfo>::iterator next_from_found;
  std::vector<MemoryInfo>::iterator prev_from_found;

  bool prev_is_found = false;
  bool next_is_found = false;

  if (memory.m_freed.size() == 1) {
    auto free_mem = memory.m_freed.begin();

    if (found_info->is_next_from_given(*free_mem)) {
      prev_from_found = free_mem;
      prev_is_found   = true;
    } else if (found_info->is_prev_from_given(*free_mem)) {
      next_from_found = free_mem;
      next_is_found   = true;
    }
  } else if (memory.m_freed.size() > 1) {
    std::sort(memory.m_freed.begin(),
              memory.m_freed.end(),
              [](const MemoryInfo& lhs, const MemoryInfo& rhs) {
                return lhs.start_index < rhs.start_index;
              });

    const size_t free_mem_sorted_length = memory.m_freed.size() - 1;

    for (size_t i = 0; i < free_mem_sorted_length; ++i) {
      if (found_info->is_next_from_given(memory.m_freed[i])) {
        prev_from_found = memory.m_freed.begin() + i;
        prev_is_found   = true;
      }

      if (found_info->is_prev_from_given(memory.m_freed[i + 1])) {
        next_from_found = memory.m_freed.begin() + (i + 1);
        next_is_found   = true;
      }

      if (!prev_is_found || !next_is_found) {
        break;
      }
    }
  }

  int num_removed = 0;

  if (!prev_is_found && !next_is_found) {
    memory.m_freed.push_back(*found_info);
  } else if (prev_is_found && !next_is_found) {
    prev_from_found->total_size += found_info->total_size;
  } else if (!prev_is_found && next_is_found) {
    found_info->total_size += next_from_found->total_size;

    memory.m_freed.erase(next_from_found);
    memory.m_freed.push_back(*found_info);
  } else {
    prev_from_found->total_size +=
      (found_info->total_size + next_from_found->total_size);

    memory.m_freed.erase(next_from_found);
    memory.m_freed.push_back(*found_info);
  }

  memory.m_allocated.erase(found_info);
}

void
release_memory()
{
  memory.m_memory.clear();
  memory.m_freed.clear();
  memory.m_allocated.clear();
}

} // namespace pxd::memory
