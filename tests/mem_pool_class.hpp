#pragma once

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <limits>
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

class MemoryPool
{
public:
  MemoryPool(size_t size)
  {
    m_memory.resize(size);

    MemoryInfo all  = {};
    all.start_index = 0;
    all.total_size  = size;

    m_freed.push_back(all);
  }
  MemoryPool()                                   = delete;
  MemoryPool(const MemoryPool& other)            = default;
  MemoryPool& operator=(const MemoryPool& other) = default;
  MemoryPool(MemoryPool&& other)                 = default;
  MemoryPool& operator=(MemoryPool&& other)      = default;
  ~MemoryPool()                                  = default;

  template<typename T>
  auto malloc(size_t size) -> T*;

  template<typename T>
  auto calloc(size_t size) -> T*;

  template<typename T>
  void free(T* mem_pointer);

private:
  std::vector<MemoryInfo> m_memory;
  std::vector<MemoryInfo> m_freed;
  std::vector<MemoryInfo> m_allocated;
};

template<typename T>
auto
MemoryPool::malloc(size_t size) -> T*
{
  size = size * sizeof(T);

  if (m_freed.empty() || size > m_memory.size()) {
    return nullptr;
  }

  if (m_freed.size() > 1) {
    std::sort(m_freed.begin(),
              m_freed.end(),
              [](const MemoryInfo& lhs, const MemoryInfo& rhs) {
                return lhs.total_size < rhs.total_size;
              });
  }

  auto selected = m_freed.end();

  for (auto iter = m_freed.begin(); iter != m_freed.end(); ++iter) {
    if (iter->total_size >= size) {
      selected = iter;
      break;
    }
  }

  if (selected == m_freed.end()) {
    return nullptr;
  }

  T* start_ptr = static_cast<T*>(&m_memory[selected->start_index]);

  MemoryInfo allocated  = {};
  allocated.start_index = selected->start_index;
  allocated.total_size  = size;

  m_allocated.push_back(allocated);

  if (selected->total_size > size) {
    selected->start_index += size;
    selected->total_size  -= size;
  } else {
    m_freed.erase(selected);
  }

  return start_ptr;
}

template<typename T>
auto
MemoryPool::calloc(size_t size) -> T*
{
  T* result = malloc<T>(size);

  if (nullptr == result) {
    return nullptr;
  }

  std::memset(result, 0, size);

  return result;
}

template<typename T>
void
MemoryPool::free(T* mem_pointer)
{
  /*
   * there are no allocations
   */
  if (m_allocated.empty()) {
    return;
  }

  std::vector<MemoryInfo>::iterator found_info_iter = m_allocated.end();

  for (auto iter = m_allocated.begin(); iter != m_allocated.end(); ++iter) {
    if (mem_pointer == static_cast<T*>(&m_memory[iter->start_index])) {
      found_info_iter = iter;
      break;
    }
  }

  if (found_info_iter == m_allocated.end()) {
    return;
  }

  MemoryInfo found_mem = *found_info_iter;
  MemoryInfo next_from_found;
  MemoryInfo prev_from_found;

  bool prev_is_found = false;
  bool next_is_found = false;

  size_t cand_prev_found_index = std::numeric_limits<size_t>::max();
  size_t cand_next_found_index = std::numeric_limits<size_t>::max();
  size_t found_index           = std::numeric_limits<size_t>::max();

  if (m_freed.size() == 1) {
    MemoryInfo single_mem = m_freed[0];

    if (found_mem.is_next_from_given(single_mem)) {
      prev_from_found       = single_mem;
      prev_is_found         = true;
      cand_prev_found_index = 0;
    } else if (found_mem.is_prev_from_given(single_mem)) {
      next_from_found       = single_mem;
      next_is_found         = true;
      cand_next_found_index = 0;
    }
  } else if (m_freed.size() > 1) {
    m_freed.push_back(found_mem);

    std::sort(m_freed.begin(),
              m_freed.end(),
              [](const MemoryInfo& lhs, const MemoryInfo& rhs) {
                return lhs.start_index < rhs.start_index;
              });

    const size_t m_freed_length = m_freed.size();

    for (size_t i = 0; i < m_freed_length; ++i) {
      if (m_freed[i] == found_mem) {
        found_index = i;
        break;
      }
    }

    if (found_index == 0) {
      cand_next_found_index = 1;
    } else if (found_index == m_freed_length) {
      cand_prev_found_index = found_index - 1;
    } else {
      cand_prev_found_index = found_index - 1;
      cand_next_found_index = found_index + 1;
    }

    if (cand_prev_found_index != std::numeric_limits<size_t>::max()) {
      if (found_mem.is_next_from_given(m_freed[cand_prev_found_index])) {
        prev_from_found = m_freed[cand_prev_found_index];
        prev_is_found   = true;
      }
    }

    if (cand_next_found_index != std::numeric_limits<size_t>::max()) {
      if (found_mem.is_prev_from_given(m_freed[cand_next_found_index])) {
        next_from_found = m_freed[cand_next_found_index];
        next_is_found   = true;
      }
    }
  }

  if (!prev_is_found && !next_is_found) {
    m_freed.push_back(found_mem);
  } else if (prev_is_found && !next_is_found) {
    m_freed[cand_prev_found_index].total_size += found_mem.total_size;
  } else if (!prev_is_found && next_is_found) {
    found_mem.total_size += next_from_found.total_size;

    m_freed.erase(m_freed.begin() + cand_next_found_index);
    m_freed.push_back(found_mem);
  } else {
    m_freed[cand_prev_found_index].total_size +=
      (found_mem.total_size + next_from_found.total_size);

    m_freed.erase(m_freed.begin() + cand_next_found_index);
  }

  if (found_index != std::numeric_limits<size_t>::max()) {
    m_freed.erase(m_freed.begin() + found_index);
  }

  m_allocated.erase(found_info_iter);
}

} // namespace pxd::memory
