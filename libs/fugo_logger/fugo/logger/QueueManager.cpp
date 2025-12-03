// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include "QueueManager.h"

#include <mutex>

namespace fugo::logger {

auto QueueManager::createProducer(std::optional<std::size_t> capacityHint) noexcept -> Queue::Producer {
  if (!capacityHint) {
    capacityHint = this->queueCapacityHint();
  }

  auto [producer, consumer] = Queue::createProducerAndConsumer("logger-queue", *capacityHint);
  if (!producer || !consumer) {
    return {};
  }

  {
    std::lock_guard guard(pendingAddQueuesLock_);
    pendingAddQueues_.push_back(std::move(consumer));
    rebuildQueuesFlag_.store(true, std::memory_order_relaxed);
  }

  return producer;
}

void QueueManager::rebuildQueues() {
  // Drop closed queues
  std::erase_if(queues_, [](Queue::Consumer const& consumer) {
    return consumer.closed();
  });

  // Add pending queues
  std::lock_guard guard(pendingAddQueuesLock_);
  if (!pendingAddQueues_.empty()) {
    for (Queue::Consumer& consumer : pendingAddQueues_) {
      queues_.push_back(std::move(consumer));
    }
    pendingAddQueues_.clear();
  }
}

} // namespace fugo::logger
