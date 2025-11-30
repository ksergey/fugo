// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <thread>

#include <fugo/core/Platform.h>

#include "Queue.h"

namespace fugo::logger {

class ThreadContext {
private:
  Queue::Producer producer_;
  std::thread::id threadID_ = std::this_thread::get_id();

public:
  ThreadContext(ThreadContext const&) = delete;
  ThreadContext& operator=(ThreadContext const&) = delete;
  ThreadContext(ThreadContext&&) = default;
  ThreadContext& operator=(ThreadContext&&) = default;

  ThreadContext() = default;

  /// Constructor
  explicit ThreadContext(Queue::Producer producer) noexcept : producer_(std::move(producer)) {
    assert(producer_);
  }

  /// Destructor
  ~ThreadContext() {
    if (producer_) {
      producer_.close();
    }
  }

  /// Get producer for queue
  [[nodiscard]] FUGO_FORCE_INLINE auto producer() noexcept -> Queue::Producer& {
    return producer_;
  }

  /// Get thread id
  [[nodiscard]] FUGO_FORCE_INLINE auto threadID() const noexcept -> std::thread::id {
    return threadID_;
  }
};

} // namespace fugo::logger
