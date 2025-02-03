#include "../includes/assert_checks.hpp"

#include <cassert>
#include <fmt/base.h>
#include <fmt/core.h>

namespace pxd {
void
assert_func(bool expression, std::source_location&& location)
{
  if (expression) {
    return;
  }

  fmt::println(
    "Assertion is FAILED at:\n    Filename : {}\n    Line     : {}\n    "
    "Function : "
    "{}\n",
    location.file_name(),
    location.line(),
    location.function_name());

  assert(false);
}

void
todo_func(const char* message, std::source_location&& location)
{
  fmt::print(
    "TODO message:\n    Message : {}\n    Filename : {}\n    Line     : "
    "{}\n    "
    "Function : "
    "{}\n",
    message,
    location.file_name(),
    location.line(),
    location.function_name());

  assert(false);
}

void
assert_msg_func(const char*            message,
                bool                   expression,
                std::source_location&& location)
{
  if (expression) {
      return;
    }

    fmt::print(
        "Assert is FAILED at: message:\n    Message : {}\n    Filename : {}\n "
        "   Line     : "
        "{}\n    "
        "Function : "
        "{}\n",
        message, location.file_name(), location.line(), location.function_name());

    assert(false);
}

} // namespace pxd
