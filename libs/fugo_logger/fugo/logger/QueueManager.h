// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <atomic>
#include <cassert>
#include <ranges>
#include <vector>

#include <fugo/core/SpinLock.h>

#include "Queue.h"

namespace fugo::logger {

/// Default queue capacity hint
constexpr std::size_t kDefaultCapacityHint = 2 * 1024 * 1024;

/// Queue manager
/// Used for queues lifetime
class QueueManager final {
private:
  std::vector<Queue::Consumer> queues_;
  std::vector<Queue::Consumer> pendingAddQueues_;
  std::vector<Queue::Consumer*> cachedQueues_;
  std::atomic<std::size_t> queueCapacityHint_{kDefaultCapacityHint};
  std::atomic<bool> rebuildQueuesCacheFlag_{false};
  SpinLock lock_;

public:
  [[nodiscard]] static auto instance() -> QueueManager* {
    static QueueManager instance;
    return &instance;
  }

  QueueManager(QueueManager const&) = delete;
  QueueManager& operator=(QueueManager const&) = delete;

  [[nodiscard]] auto queueCapacityHint() const noexcept -> std::size_t {
    return queueCapacityHint_.load(std::memory_order_relaxed);
  }

  /// Set a queue capacity hint (for new queues)
  /// @throw std::runtime_error on value is zero
  void setQueueCapacityHint(std::size_t value) {
    if (value == 0) [[unlikely]] {
      throw std::runtime_error("queue capacity hint is out of range");
    }
    queueCapacityHint_.store(value, std::memory_order_relaxed);
  }

  /// Create producer
  [[nodiscard]] auto createProducer() noexcept -> Queue::Producer;

  template <typename Fn>
    requires std::invocable<Fn, Queue::Consumer*>
  void forEachConsumer(Fn&& fn) {
    if (rebuildQueuesCacheFlag_.load(std::memory_order_relaxed)) [[unlikely]] {
      this->rebuildQueuesCache();
      rebuildQueuesCacheFlag_.store(false, std::memory_order_relaxed);
    }
    for (auto const consumer : cachedQueues_) {
      assert(!consumer->isClosed());
      std::invoke(std::forward<Fn>(fn), consumer);
      if (consumer->isClosed()) [[unlikely]] {
        this->notifyRebuildQueuesCache();
      }
    }
  }

private:
  QueueManager() = default;

  void notifyRebuildQueuesCache() noexcept {
    rebuildQueuesCacheFlag_.store(true, std::memory_order_relaxed);
  }

  void rebuildQueuesCache();
};

} // namespace fugo::logger
