// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>

#include <fmt/format.h>

#include "Common.h"
#include "Sink.h"
#include "ThreadContextManager.h"

namespace fugo::logger {

// TODO: refactoring, split?

class LoggerContext {
private:
  ThreadContextManager threadContextManager_;
  // Current max verbosity level
  std::atomic<LogLevel> logLevel_{LogLevel::Notice};
  // Sink
  std::atomic<std::shared_ptr<Sink>> sink_;
  // Cache for message formatting
  fmt::memory_buffer formatBuffer_;
  // Guard for thread start
  std::atomic<std::once_flag*> startOnceFlag_;
  // Backend thread
  std::jthread backendThread_;
  // Flag indicates backend thread is running
  std::atomic<bool> backendThreadRunning_ = false;

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

  /// Return true on backend thread running
  [[nodiscard]] FUGO_FORCE_INLINE auto backendThreadRunning() const noexcept {
    return backendThreadRunning_.load(std::memory_order_acquire);
  }

  /// Start backend background thread
  void startBackendThread();

  /// Stop backend background thread
  void stopBackendThread();

  // TODO: make private
  void backendWork();

private:
  LoggerContext();
  ~LoggerContext();

  void processLogRecord(
      Sink* sink, LogRecordHeader const* logRecordHeader, RecordMetadata const* metadata, std::byte const* argsBuffer);

  void runBackendThread();
};

/// Alias for LoggerContext::instance()
inline auto loggerContext() noexcept -> LoggerContext* {
  return LoggerContext::instance();
}

} // namespace fugo::logger
