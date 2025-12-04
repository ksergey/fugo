// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include <doctest/doctest.h>

#include <chrono>
#include <ranges>
#include <thread>

#include "DailyFileSink.h"
#include "Logger.h"
#include "StdOutSink.h"

namespace fugo::logger {

TEST_CASE("Logger: ohm") {
  REQUIRE_FALSE(fugo::logger::isBackendReady());

  fugo::logger::setLogLevel("trace");

  fugo::logger::startBackend(std::make_unique<StdOutSink>());
  REQUIRE(fugo::logger::isBackendReady());

  REQUIRE_EQ(fugo::logger::logLevel(), LogLevel::Trace);

  logNotice("Hello {}!", "world");

  fugo::logger::stopBackend();
}

TEST_CASE("Logger: DailyFileSink") {
  REQUIRE_FALSE(fugo::logger::isBackendReady());

  fugo::logger::startBackend(std::make_unique<DailyFileSink>());
  REQUIRE(fugo::logger::isBackendReady());

  logWarningF("begin");
  for (auto i : std::views::iota(1) | std::views::take(50)) {
    logWarningF("record #{}", i);
  }
  logWarningF("end");

  fugo::logger::stopBackend();
}

} // namespace fugo::logger
