#pragma once

#include <fmt/base.h>
#include <fmt/os.h>
#include <source_location>

namespace pxd {

class Logger
{
  Logger() = default;
  ~Logger() noexcept;

public:
  Logger(const Logger& other)                    = delete;
  auto operator=(const Logger& other) -> Logger& = delete;
  Logger(Logger&& other)                         = delete;
  auto operator=(Logger&& other) -> Logger&      = delete;

  template<typename... T>
  void info(fmt::format_string<T...> msg,
            std::source_location&&   location,
            T&&... args)
  {
    log("INFO",
        msg,
        location.file_name(),
        location.line(),
        location.function_name(),
        fmt::make_format_args(args...));
  }

  template<typename... T>
  void warning(fmt::format_string<T...> msg,
               std::source_location&&   location,
               T&&... args)
  {
    log("WARNING",
        msg,
        location.file_name(),
        location.line(),
        location.function_name(),
        fmt::make_format_args(args...));
  }

  template<typename... T>
  void error(fmt::format_string<T...> msg,
             std::source_location&&   location,
             T&&... args)
  {
    log("ERROR",
        msg,
        location.file_name(),
        location.line(),
        location.function_name(),
        fmt::make_format_args(args...));
  }

  static auto get_instance() noexcept -> Logger*;

private:
  void log(fmt::string_view log_level,
           fmt::string_view msg,
           fmt::string_view filename,
           int              line,
           fmt::string_view func_name,
           fmt::format_args args);

  fmt::v11::ostream log_file = fmt::output_file("app.log");
  static Logger*    instance;
};
} // namespace pxd

#ifndef PXD_LOG_NOWHERE
#define PXD_LOG_INFO(msg, ...)                                                 \
  {                                                                            \
    pxd::Logger* logger = pxd::Logger::get_instance();                         \
    logger->info(msg, std::source_location::current(), ##__VA_ARGS__);         \
  }
#define PXD_LOG_WARNING(msg, ...)                                              \
  {                                                                            \
    pxd::Logger* logger = pxd::Logger::get_instance();                         \
    logger->warning(msg, std::source_location::current(), ##__VA_ARGS__);      \
  }
#define PXD_LOG_ERROR(msg, ...)                                                \
  {                                                                            \
    pxd::Logger* logger = pxd::Logger::get_instance();                         \
    logger->error(msg, std::source_location::current(), ##__VA_ARGS__);        \
  }
#else
#define PXD_LOG_INFO(msg, ...)
#define PXD_LOG_WARNING(msg, ...)
#define PXD_LOG_ERROR(msg, ...)
#endif
