// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include "StdOutSink.h"

#include <ctime>

#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/std.h>

namespace fugo::logger {

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

  auto const data = formatter_(location, level, timestamp, threadID, message);

  fmt::print(style, "{}\n", data);
}

void StdOutSink::flush() {
  std::fflush(stdout);
}

} // namespace fugo::logger
