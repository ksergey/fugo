// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <atomic>
#include <memory>

#include <fmt/format.h>

#include "Common.h"
#include "Sink.h"
#include "ThreadContextManager.h"

namespace fugo::logger {

class LoggerContext {
private:
  ThreadContextManager threadContextManager_;
  std::atomic<LogLevel> logLevel_{LogLevel::Notice};
  std::atomic<std::shared_ptr<Sink>> sink_;

  // cache for message formatting
  fmt::memory_buffer formatBuffer_;

public:
  static auto instance() noexcept -> LoggerContext* {
    static LoggerContext instance;
    return &instance;
  }

  LoggerContext(LoggerContext const&) = delete;
  LoggerContext& operator=(LoggerContext const&) = delete;
  LoggerContext(LoggerContext&&) = delete;
  LoggerContext& operator=(LoggerContext&&) = delete;

  /// Current log verbosity level
  [[nodiscard]] auto logLevel() const noexcept {
    return logLevel_.load(std::memory_order_relaxed);
  }

  /// Change log verbosity level
  void setLogLevel(LogLevel value) noexcept {
    logLevel_.store(value, std::memory_order_relaxed);
  }

  /// Return true on message with log verbosity value @c value should be logged
  [[nodiscard]] auto shouldLog(LogLevel value) const noexcept {
    return value <= this->logLevel();
  }

  /// Logging queue capacity hint (bytes)
  [[nodiscard]] auto queueCapacityHint() const noexcept {
    return threadContextManager_.queueCapacityHint();
  }

  /// Change logging queue capacity hint (bytes)
  void setQueueCapacityHint(std::size_t value) {
    threadContextManager_.setQueueCapacityHint(value);
  }

  /// Get current sink
  [[nodiscard]] auto sink() const noexcept -> std::shared_ptr<Sink> {
    return sink_.load(std::memory_order_relaxed);
  }

  /// Update sink
  void setSink(std::shared_ptr<Sink> const& sink) {
    sink_.store(sink, std::memory_order_release);
  }

  /// Return thread context for current thread
  [[nodiscard]] auto currentThreadContext() noexcept -> ThreadContext* {
    static thread_local ThreadContext threadContext{threadContextManager_.createThreadContext()};
    return &threadContext;
  }

  void backendWork();

private:
  LoggerContext();

  ~LoggerContext() = default;

  void processLogRecord(
      Sink* sink, LogRecordHeader const* logRecordHeader, RecordMetadata const* metadata, std::byte const* argsBuffer);
};

inline auto loggerContext() noexcept -> LoggerContext* {
  return LoggerContext::instance();
}

} // namespace fugo::logger
