// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include "QueueManager.h"

#include <mutex>

namespace fugo::logger {

auto QueueManager::createProducer() noexcept -> Queue::Producer {
  auto [producer, consumer] = Queue::createProducerAndConsumer("logger-queue", this->queueCapacityHint());
  if (!producer || !consumer) {
    return {};
  }

  {
    std::lock_guard guard(lock_);
    pendingAddQueues_.push_back(std::move(consumer));
    this->notifyRebuildQueuesCache();
  }

  return producer;
}

void QueueManager::rebuildQueuesCache() {
  std::lock_guard guard(lock_);

  // Drop closed queues
  std::erase_if(queues_, [](Queue::Consumer const& consumer) {
    return consumer.closed();
  });

  // Add pending queues
  if (!pendingAddQueues_.empty()) {
    for (Queue::Consumer& consumer : pendingAddQueues_) {
      queues_.push_back(std::move(consumer));
    }
    pendingAddQueues_.clear();
  }

  cachedQueues_.resize(queues_.size());
  std::ranges::transform(queues_, cachedQueues_.begin(), [](Queue::Consumer& consumer) {
    return &consumer;
  });
}

} // namespace fugo::logger
