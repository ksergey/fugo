// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <ranges>
#include <vector>

#include <fugo/core/SpinLock.h>

#include "ThreadContext.h"

namespace fugo::logger {

class ThreadContextManager {
private:
  SpinLock lock_;
  std::vector<Queue::Consumer> storage_;
  std::vector<Queue::Consumer> pending_;
  std::vector<Queue::Consumer*> cached_;
  std::atomic<std::size_t> queueCapacityHint_ = 4 * 1024 * 1024;
  std::atomic<bool> updateCache_ = false;

public:
  ThreadContextManager(ThreadContextManager const&) = delete;
  ThreadContextManager& operator=(ThreadContextManager const&) = delete;

  ThreadContextManager();
  ~ThreadContextManager();

  /// Queue capacity hint (only for new queues, thread-safe)
  [[nodiscard]] auto queueCapacityHint() const noexcept -> std::size_t {
    return queueCapacityHint_.load(std::memory_order_relaxed);
  }

  /// Set queue capacity hint (thread-safe)
  void setQueueCapacityHint(std::size_t value) {
    if (value == 0) {
      throw std::runtime_error("queue capacity hint is out of range");
    }
    queueCapacityHint_.store(value, std::memory_order_relaxed);
  }

  /// Create ThreadContext (thread-safe)
  [[nodiscard]] auto createThreadContext() noexcept -> ThreadContext;

  /// Get active queues
  /// The method should be called from backend
  [[nodiscard]] auto queues() {
    if (updateCache_.load(std::memory_order_relaxed)) [[unlikely]] {
      this->doUpdateCache();
      updateCache_.store(false, std::memory_order_relaxed);
    }
    return cached_ | std::views::transform([](Queue::Consumer* consumerPtr) -> Queue::Consumer& {
      return *consumerPtr;
    });
  }

  /// Process pending create/destroy queues
  void invalidateQueues() noexcept {
    updateCache_.store(true, std::memory_order_relaxed);
  }

private:
  void doUpdateCache();
};

} // namespace fugo::logger
