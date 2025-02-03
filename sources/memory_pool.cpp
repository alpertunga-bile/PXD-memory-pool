#include "../includes/memory_pool.hpp"

#include <cstdint>
#include <list>
#include <vector>

namespace pxd {

class MemoryPool
{
private:
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

public:
  MemoryPool() = delete;
  explicit MemoryPool(size_t size);
  MemoryPool(const MemoryPool& other)            = default;
  MemoryPool& operator=(const MemoryPool& other) = default;
  MemoryPool(MemoryPool&& other)                 = default;
  MemoryPool& operator=(MemoryPool&& other)      = default;
  ~MemoryPool() noexcept                         = default;

private:
  std::vector<uint8_t>  m_memory;
  std::list<MemoryInfo> m_free_memories;
};

MemoryPool::MemoryPool(size_t size)
{
  m_memory.resize(size);
}

} // namespace pxd
