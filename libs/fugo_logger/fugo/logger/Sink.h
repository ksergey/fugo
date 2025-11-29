// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <fmt/format.h>

#include <string_view>

#include "Common.h"

namespace fugo::logger {

/// Logger sink interface
struct Sink {
  virtual ~Sink() = default;

  /// Write log entry
  virtual void write(std::source_location const& location, LogLevel level, ::timespec const& timestamp,
      std::thread::id const& threadID, std::string_view message) = 0;

  /// Flush
  virtual void flush() {}
};

constexpr auto toShortString(LogLevel level) noexcept -> std::string_view {
  using namespace std::string_view_literals;

  switch (level) {
  case LogLevel::Error: return "E"sv;
  case LogLevel::Warning: return "W"sv;
  case LogLevel::Notice: return "I"sv;
  case LogLevel::Debug: return "D"sv;
  case LogLevel::Trace: return "T"sv;
  default: break;
  }

  return ""sv;
}

} // namespace fugo::logger
