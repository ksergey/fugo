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

#if 0
TEST_CASE("Logger: basic") {
  loggerContext()->setQueueCapacityHint(256);

  auto thread = std::jthread([] {
    logWarningF("begin");
    for (auto i : std::views::iota(1) | std::views::take(50)) {
      logWarningF("record #{}", i);
    }
    logWarningF("end");
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  loggerContext()->backendWork();

  std::this_thread::sleep_for(std::chrono::milliseconds(250));
  loggerContext()->backendWork();

  thread.join();
}

TEST_CASE("Logger: fun") {
  loggerContext()->setQueueCapacityHint(1024 * 1024 * 10);

  std::jthread([] {
    logWarning("This is warning ({})", 7782);
  }).join();

  logError("Hello {}", "world");
  logNotice("Hello {} {}", 512, 3.14);

  auto thread1 = std::jthread([] {
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    logNotice("hello world");
  });

  auto thread2 = std::jthread([] {
    logNotice("sweet dreams are made of {}", "this");
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
  });

  logNotice("-----------------");
  loggerContext()->backendWork();

  thread1.join();
  thread2.join();

  logNotice("-----------------");
  loggerContext()->backendWork();
}
#endif

TEST_CASE("Logger: ohm") {
  REQUIRE_FALSE(fugo::logger::backendThreadRunning());

  fugo::logger::setQueueCapacityHint(1024 * 1024);
  fugo::logger::startBackendThread(std::make_unique<StdOutSink>());
  REQUIRE(fugo::logger::backendThreadRunning());

  fugo::logger::setLogLevel("trace");

  REQUIRE_EQ(fugo::logger::logLevel(), LogLevel::Trace);

  logNotice("Hello {}!", "world");

  fugo::logger::stopBackendThread();
}

TEST_CASE("Logger: DailyFileSink") {
  REQUIRE_FALSE(fugo::logger::backendThreadRunning());

  fugo::logger::startBackendThread(std::make_unique<DailyFileSink>());

  REQUIRE(fugo::logger::backendThreadRunning());

  logWarningF("begin");
  for (auto i : std::views::iota(1) | std::views::take(50)) {
    logWarningF("record #{}", i);
  }
  logWarningF("end");

  fugo::logger::stopBackendThread();
}

} // namespace fugo::logger
