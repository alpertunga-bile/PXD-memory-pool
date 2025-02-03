#pragma once

#include <source_location>

constexpr auto
IS_VALID_PTR(void* obj) -> bool
{
  return obj != nullptr;
}

namespace pxd {
void
assert_func(bool expression, std::source_location&& location);
void
todo_func(const char* message, std::source_location&& location);
void
assert_msg_func(const char*            message,
                bool                   expression,
                std::source_location&& location);
} // namespace pxd

#if defined(PXD_ENABLE_ASSERTS)
#define PXD_ASSERT(exp) pxd::assert_func(exp, std::source_location::current())
#define PXD_ASSERT_MSG(exp, msg)                                               \
  pxd::assert_msg_func(msg, exp, std::source_location::current())
#define PXD_TODO(msg) pxd::todo_func(msg, std::source_location::current())
#else
#define PXD_ASSERT(exp)
#define PXD_ASSERT_MSG(exp, msg)
#define PXD_TODO(msg)
#endif
