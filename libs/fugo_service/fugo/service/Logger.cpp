// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include "Logger.h"

#include <unistd.h>

#include <fugo/logger/DailyFileSink.h>
#include <fugo/logger/StdOutSink.h>

namespace fugo::service {
namespace {

void initLogger(std::unique_ptr<fugo::logger::Sink> sink, LoggerOptions const& options) {
  auto const backendOptions =
      fugo::logger::BackendOptions{.bindToCoreNo = options.bindToCoreNo, .sleepDuration = options.sleepDuration};
  fugo::logger::setQueueCapacityHint(options.queueCapacityHint * 1024 * 1024);
  fugo::logger::setLogLevel(options.logLevel);
  fugo::logger::startBackend(std::move(sink), backendOptions);
}

[[nodiscard]] auto getBinaryPath() noexcept -> std::expected<std::filesystem::path, std::error_code> {
  char buffer[PATH_MAX];
  if (auto const rc = ::readlink("/proc/self/exe", buffer, sizeof(buffer)); rc != -1) {
    return std::filesystem::path{std::string{buffer, static_cast<std::size_t>(rc)}};
  } else {
    return std::unexpected(makePosixErrorCode(errno));
  }
}

} // namespace

void initLoggerToStdOut(LoggerOptions const& options) {
  auto sink = std::make_unique<fugo::logger::StdOutSink>();
  sink->setPattern(options.pattern);
  initLogger(std::move(sink), options);
}

void initLoggerToFile(std::filesystem::path const& destPath, LoggerOptions const& options) {
  auto sink = std::make_unique<fugo::logger::DailyFileSink>(destPath, getBinaryPath().value().filename().native());
  sink->setPattern(options.pattern);
  initLogger(std::move(sink), options);
}

void shutdownLogger() noexcept {
  fugo::logger::stopBackend();
}

} // namespace fugo::service
