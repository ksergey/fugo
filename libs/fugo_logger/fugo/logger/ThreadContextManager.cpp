// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include "ThreadContextManager.h"

#include <algorithm>

namespace fugo::logger {

ThreadContextManager::ThreadContextManager() {
  storage_.reserve(8);
  pending_.reserve(8);
  cached_.reserve(8);
}

ThreadContextManager::~ThreadContextManager() {}

auto ThreadContextManager::createThreadContext() noexcept -> ThreadContext {
  auto [producer, consumer] = Queue::createProducerAndConsumer("logger-queue", this->queueCapacityHint());
  if (!producer || !consumer) {
    return ThreadContext();
  }

  {
    std::lock_guard guard(lock_);
    pending_.push_back(std::move(consumer));
    this->invalidateQueues();
  }

  return ThreadContext(std::move(producer));
}

void ThreadContextManager::doUpdateCache() {
  std::lock_guard guard(lock_);

  // Drop closed queues
  std::erase_if(storage_, [](Queue::Consumer const& consumer) {
    return consumer.closed();
  });

  // Add pending queues
  if (!pending_.empty()) {
    for (Queue::Consumer& consumer : pending_) {
      storage_.push_back(std::move(consumer));
    }
    pending_.clear();
  }

  cached_.resize(storage_.size());
  std::ranges::transform(storage_, cached_.begin(), [](Queue::Consumer& consumer) {
    return &consumer;
  });
}

} // namespace fugo::logger
