#include "../includes/memory_pool.hpp"

namespace pxd {

MemoryPool::MemoryPool(size_t size) {
  m_memory.resize(size);
}

} // namespace pxd
