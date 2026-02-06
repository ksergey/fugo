// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <thread>

#include <fmt/format.h>

#include <fugo/core/Platform.h>

#include "LoggerQueue.h"
#include "LoggerQueueManager.h"

namespace fugo::logger::detail {

class ThreadContext final {
  private:
    LoggerQueue::Producer producer_;
    std::thread::id threadID_;

  public:
    ThreadContext(ThreadContext const&) = delete;
    ThreadContext& operator=(ThreadContext const&) = delete;
    ThreadContext(ThreadContext&&) = default;
    ThreadContext& operator=(ThreadContext&&) = default;

    /// Constructor
    ThreadContext(LoggerQueueManager& loggerQueueManager) noexcept
        : producer_{loggerQueueManager.createProducer()}, threadID_{std::this_thread::get_id()} {}

    /// Destructor
    ~ThreadContext() {
        if (producer_) {
            producer_.close();
        }
    }

    /// Get producer for queue
    [[nodiscard]] FUGO_FORCE_INLINE auto producer() noexcept -> LoggerQueue::Producer& {
        return producer_;
    }

    /// Get thread id
    [[nodiscard]] FUGO_FORCE_INLINE auto threadID() const noexcept -> std::thread::id {
        return threadID_;
    }
};

} // namespace fugo::logger::detail
