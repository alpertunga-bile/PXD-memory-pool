#include "../includes/logger.hpp"

#include <filesystem>
#include <fmt/base.h>
#include <fmt/core.h>
#include <fmt/os.h>

namespace pxd {

Logger* Logger::instance = nullptr;

Logger::~Logger() noexcept
{
  log_file.close();

  if (instance == nullptr) {
    return;
  }

  delete instance;
  instance = nullptr;
}

auto
Logger::get_instance() noexcept -> Logger*
{
  if (instance == nullptr) {
    try {
      instance = new Logger();
    } catch (const std::exception& e) {
      fmt::println("::Logger::Error::{}", e.what());
      return nullptr;
    }
  }

  return instance;
}

void
Logger::log(fmt::string_view log_level,
            fmt::string_view msg,
            fmt::string_view filename,
            int              line,
            fmt::string_view func_name,
            fmt::format_args args)
{
#ifndef PXD_LOG_NOWHERE

  fmt::string_view base_filename =
    std::filesystem::path(filename.data()).filename().string();

  const char* format_str =
    "[{:8s}] /_\\ {:50s} /_\\ {:20s} /_\\ {:5d} /_\\ {}\n";

#ifndef PXD_LOG_FILE_ONLY
  fmt::print(fmt::runtime(format_str),
             log_level,
             fmt::vformat(msg, args),
             base_filename,
             line,
             func_name);
#endif

  log_file.print(fmt::runtime(format_str),
                 log_level,
                 fmt::vformat(msg, args),
                 base_filename,
                 line,
                 func_name);
}

#endif

} // namespace pxd
