// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <fugo/core/Platform.h>

#include "BackendThread.h"
#include "Common.h"
#include "Sink.h"
#include "ThreadContextManager.h"

namespace fugo::logger {

class Runtime {
private:
  ThreadContextManager threadContextManager_;
  BackendThread backendThread_{threadContextManager_};

  alignas(kHardwareDestructiveInterferenceSize) std::atomic<LogLevel> logLevel_ = LogLevel::Notice;

public:
  [[nodiscard]] FUGO_FORCE_INLINE static auto instance() -> Runtime& {
    static Runtime instance;
    return instance;
  }

  Runtime(Runtime const&) = delete;
  Runtime& operator=(Runtime const&) = delete;

  /// Current log verbosity level
  [[nodiscard]] FUGO_FORCE_INLINE auto logLevel() const noexcept {
    return logLevel_.load(std::memory_order_relaxed);
  }

  /// Change log verbosity level
  FUGO_FORCE_INLINE void setLogLevel(LogLevel value) noexcept {
    logLevel_.store(value, std::memory_order_relaxed);
  }

  /// Return true on message with log verbosity value @c value should be logged
  [[nodiscard]] FUGO_FORCE_INLINE auto shouldLog(LogLevel value) const noexcept {
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

  /// Return thread context for current thread
  [[nodiscard]] FUGO_FORCE_INLINE auto localThreadContext() noexcept -> ThreadContext& {
    static thread_local ThreadContext threadContext{threadContextManager_.createThreadContext()};
    return threadContext;
  }

  /// Return true on backend thread running
  [[nodiscard]] FUGO_FORCE_INLINE auto isBackendThreadRunning() const noexcept -> bool {
    return backendThread_.isRunning();
  }

  /// Start backend thread
  FUGO_FORCE_INLINE void startBackendThread(std::unique_ptr<Sink> sink) {
    backendThread_.start(std::move(sink));
  }

  /// Stop backend thread
  FUGO_FORCE_INLINE void stopBackendThread() {
    backendThread_.stop();
  }

private:
  Runtime() = default;
};

[[nodiscard]] FUGO_FORCE_INLINE auto runtime() -> Runtime& {
  return Runtime::instance();
}

} // namespace fugo::logger
