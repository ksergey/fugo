// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include "DailyFileSink.h"

namespace fugo::logger {

DailyFileSink::DailyFileSink(std::filesystem::path const& destination, std::string prefix)
    : destination_(destination), prefix_(std::move(prefix)) {}

void DailyFileSink::write(std::source_location const& location, LogLevel level, ::timespec const& timestamp,
    std::thread::id const& threadID, std::string_view message) {

  // auto const data = formatter_(location, level, timestamp, threadID, message);
  // fmt::print("{}\n", data);
}

void DailyFileSink::flush() {}

} // namespace fugo::logger
