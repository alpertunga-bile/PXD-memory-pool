#include "../includes/memory_pool.hpp"

#include <algorithm>
#include <cstdint>
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

  [[nodiscard]] auto empty() const noexcept -> bool { return total_size == 0; }

  [[nodiscard]] auto is_prev_from_given(const MemoryInfo& other) const noexcept
    -> bool
  {
    return (start_index + total_size) == other.start_index;
  }

  [[nodiscard]] auto is_next_from_given(const MemoryInfo& other) const noexcept
    -> bool
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

[[nodiscard]] auto
malloc(size_t size) noexcept -> void*
{
  if (memory.m_freed.empty() || size > memory.m_memory.size()) {
    return nullptr;
  }

  if (memory.m_freed.size() > 1) {
    std::ranges::sort(memory.m_freed,
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

[[nodiscard]] auto
calloc(size_t size) noexcept -> void*
{
  void* result = malloc(size);

  if (nullptr == result) {
    return nullptr;
  }

  std::memset(result, 0, size);

  return result;
}

struct AdjacentsInfo
{
  MemoryInfo next_from_found;
  MemoryInfo prev_from_found;
  size_t     next_index  = std::numeric_limits<std::size_t>::max();
  size_t     prev_index  = std::numeric_limits<std::size_t>::max();
  size_t     found_index = std::numeric_limits<std::size_t>::max();
};

auto
find_adjacents(const MemoryInfo& found_mem) -> AdjacentsInfo
{
  AdjacentsInfo     adj_info;
  const std::size_t frees_length = memory.m_freed.size();

  if (frees_length == 0) {
    return adj_info;
  }

  if (frees_length == 1) {
    MemoryInfo single_mem = memory.m_freed[0];

    if (found_mem.is_next_from_given(single_mem)) {
      adj_info.prev_from_found = single_mem;
      adj_info.prev_index      = 0;
    } else if (found_mem.is_prev_from_given(single_mem)) {
      adj_info.next_from_found = single_mem;
      adj_info.next_index      = 0;
    }

    return adj_info;
  }

  std::size_t cand_prev_found_index = std::numeric_limits<std::size_t>::max();
  std::size_t cand_next_found_index = std::numeric_limits<std::size_t>::max();

  memory.m_freed.push_back(found_mem);

  std::ranges::sort(memory.m_freed,
                    [](const MemoryInfo& lhs, const MemoryInfo& rhs) {
                      return lhs.start_index < rhs.start_index;
                    });

  const std::size_t frees_length_plus_one = frees_length + 1;

  for (std::size_t i = 0; i < frees_length_plus_one; ++i) {
    if (memory.m_freed[i] == found_mem) {
      adj_info.found_index = i;
      break;
    }
  }

  if (adj_info.found_index == 0) {
    cand_next_found_index = 1;
  } else if (adj_info.found_index == frees_length) {
    cand_prev_found_index = adj_info.found_index - 1;
  } else {
    cand_prev_found_index = adj_info.found_index - 1;
    cand_next_found_index = adj_info.found_index + 1;
  }

  if (cand_prev_found_index != std::numeric_limits<size_t>::max()) {
    if (found_mem.is_next_from_given(memory.m_freed[cand_prev_found_index])) {
      adj_info.prev_from_found = memory.m_freed[cand_prev_found_index];
      adj_info.prev_index      = cand_prev_found_index;
    }
  }

  if (cand_next_found_index != std::numeric_limits<size_t>::max()) {
    if (found_mem.is_prev_from_given(memory.m_freed[cand_next_found_index])) {
      adj_info.next_from_found = memory.m_freed[cand_next_found_index];
      adj_info.next_index      = cand_next_found_index;
    }
  }

  return adj_info;
}

void
free(void* mem_pointer) noexcept
{
  if (memory.m_allocated.empty()) {
    return;
  }

  auto found_info_iter = memory.m_allocated.end();

  for (auto iter = memory.m_allocated.begin(); iter != memory.m_allocated.end();
       ++iter) {
    if (mem_pointer ==
        static_cast<void*>(&memory.m_memory[iter->start_index])) {
      found_info_iter = iter;
      break;
    }
  }

  if (found_info_iter == memory.m_allocated.end()) {
    return;
  }

  MemoryInfo found_mem = *found_info_iter;

  AdjacentsInfo adj_info = find_adjacents(found_mem);

  const bool prev_is_found = !adj_info.prev_from_found.empty();
  const bool next_is_found = !adj_info.next_from_found.empty();

  if (!prev_is_found && !next_is_found) {
    memory.m_freed.push_back(found_mem);
  } else if (prev_is_found && !next_is_found) {
    memory.m_freed[adj_info.prev_index].total_size += found_mem.total_size;
  } else if (!prev_is_found && next_is_found) {
    found_mem.total_size += adj_info.next_from_found.total_size;

    memory.m_freed.erase(memory.m_freed.begin() + adj_info.next_index);
    memory.m_freed.push_back(found_mem);
  } else {
    memory.m_freed[adj_info.prev_index].total_size +=
      (found_mem.total_size + adj_info.next_from_found.total_size);

    memory.m_freed.erase(memory.m_freed.begin() + adj_info.next_index);
  }

  if (adj_info.found_index != SIZE_MAX) {
    memory.m_freed.erase(memory.m_freed.begin() + adj_info.found_index);
  }

  memory.m_allocated.erase(found_info_iter);
}

void
release_memory() noexcept
{
  memory.m_memory.clear();
  memory.m_freed.clear();
  memory.m_allocated.clear();
}

auto
total_free_memory() -> size_t
{
  const size_t vec_length = memory.m_freed.size();

  if (vec_length == 0) {
    return 0;
  }

  size_t total_memory = 0;

  for (size_t i = 0; i < vec_length; ++i) {
    total_memory += memory.m_freed[i].total_size;
  }

  return total_memory;
}

auto
total_allocated_memory() -> size_t
{
  const size_t vec_length = memory.m_allocated.size();

  if (vec_length == 0) {
    return 0;
  }

  size_t total_memory = 0;

  for (size_t i = 0; i < vec_length; ++i) {
    total_memory += memory.m_allocated[i].total_size;
  }

  return total_memory;
}

auto
max_free_memory() -> size_t
{
  const size_t vec_length = memory.m_freed.size();

  if (vec_length == 0) {
    return 0;
  }

  size_t max_memory = 0;

  for (size_t i = 0; i < vec_length; ++i) {
    max_memory = std::max(memory.m_freed[i].total_size, max_memory);
  }

  return max_memory;
}

auto
min_free_memory() -> size_t
{
  const size_t vec_length = memory.m_freed.size();

  if (vec_length == 0) {
    return 0;
  }

  size_t min_memory = std::numeric_limits<std::size_t>::max();

  for (size_t i = 0; i < vec_length; ++i) {
    min_memory = std::min(memory.m_freed[i].total_size, min_memory);
  }

  return min_memory;
}

} // namespace pxd::memory
