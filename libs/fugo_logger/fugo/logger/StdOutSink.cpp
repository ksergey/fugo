// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include "StdOutSink.h"

#include <ctime>

#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/std.h>

namespace fugo::logger {
namespace {

// Wrapper around localtime_r
auto localtime(std::time_t time) noexcept -> ::tm {
  ::tm tm;
  return *::localtime_r(&time, &tm);
}

} // namespace

void StdOutSink::write(std::source_location const& location, LogLevel level, ::timespec const& timestamp,
    std::thread::id const& threadID, std::string_view message) {

  auto const style = [&]() noexcept -> fmt::text_style {
    switch (level) {
    case LogLevel::Error: return fg(fmt::color::red);
    case LogLevel::Warning: return fg(fmt::color::orange);
    default: break;
    }
    return fmt::text_style();
  }();

  // Format timestamp
  char buffer[30];
  auto const result =
      fmt::format_to_n(buffer, sizeof(buffer), "{:%F %T}.{:09}", localtime(timestamp.tv_sec), timestamp.tv_nsec);
  auto const timestampStr = std::string_view(buffer, result.size);

  fmt::print(style, fmt::runtime(pattern_), fmt::arg("timestamp", timestampStr), fmt::arg("threadID", threadID),
      fmt::arg("level", toShortString(level)), fmt::arg("message", message), fmt::arg("file", location.file_name()),
      fmt::arg("line", location.line()));
  fmt::print("\n");
}

void StdOutSink::flush() {
  std::fflush(stdout);
}

} // namespace fugo::logger
