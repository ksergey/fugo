// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <functional>
#include <string_view>
#include <thread>
#include <tuple>

#include <fmt/format.h>

#include <fugo/core/Platform.h>
#include <fugo/core/TypeTraits.h>
#include <turboq/BoundedSPSCRawQueue.h>

#include "Codec.h"
#include "Common.h"

namespace fugo::logger {

/// Enqueue policy
enum class EnqueuePolicy {
  Drop, ///< Drop on no space in queue
  Retry ///< Try again
};

struct Queue {
  /// Queue producer
  struct Producer : public turboq::BoundedSPSCRawQueue::Producer {
    using turboq::BoundedSPSCRawQueue::Producer::Producer;

    Producer(turboq::BoundedSPSCRawQueue::Producer producer) noexcept
        : turboq::BoundedSPSCRawQueue::Producer(std::move(producer)) {}

    /// Return true on producer side closed
    [[nodiscard]] auto closed() const noexcept -> bool {
      return static_cast<bool>(*this) == false;
    }
    [[nodiscard]] auto isClosed() const noexcept -> bool {
      return static_cast<bool>(*this) == false;
    }

    /// Close producer side and notify consumer side
    void close() {
      constexpr auto bufferSize = Codec<RecordHeader>::encodedSize();
      this->enqueue<EnqueuePolicy::Retry>(bufferSize, [](std::byte* dst) noexcept {
        Codec<RecordHeader>::encode(dst, RecordHeader{.type = EventType::Close});
      });

      // Destroy producer
      *this = {};
    }

    // TODO: OverflowPolicy
    /// Enqueue a data into queue
    template <EnqueuePolicy Policy = EnqueuePolicy::Drop, typename Fn>
    FUGO_FORCE_INLINE auto enqueue(std::size_t size, Fn&& fn) -> bool {
      auto buffer = this->prepare(size);

      if constexpr (Policy == EnqueuePolicy::Drop) {
        if (buffer.empty()) [[unlikely]] {
          return false;
        }
      } else if constexpr (Policy == EnqueuePolicy::Retry) {
        if (buffer.empty()) [[unlikely]] {
          do {
            std::this_thread::yield();
            buffer = this->prepare(size);
          } while (buffer.empty());
        }
      } else {
        static_assert(FalseV<Policy>, "Unknown EnqueuePolicy value");
      }

      std::invoke(std::forward<Fn>(fn), buffer.data());
      this->commit();
      return true;
    }
  };

  /// Queue consumer
  struct Consumer : public turboq::BoundedSPSCRawQueue::Consumer {
    using turboq::BoundedSPSCRawQueue::Consumer::Consumer;

    Consumer(turboq::BoundedSPSCRawQueue::Consumer consumer) noexcept
        : turboq::BoundedSPSCRawQueue::Consumer(std::move(consumer)) {}

    /// Return true on consumer side closed
    [[nodiscard]] auto closed() const noexcept -> bool {
      return static_cast<bool>(*this) == false;
    }
    [[nodiscard]] auto isClosed() const noexcept -> bool {
      return static_cast<bool>(*this) == false;
    }

    /// Close consumer side
    void close() noexcept {
      *this = {};
    }

    template <typename Fn>
    FUGO_FORCE_INLINE auto dequeue(Fn&& fn) -> bool {
      auto buffer = this->fetch();
      if (buffer.empty()) [[unlikely]] {
        return false;
      }
      std::invoke(std::forward<Fn>(fn), buffer.data());
      this->consume();
      return true;
    }
  };

  /// Create producer and consumer
  /// @param[in] name is queue name
  /// @param[in] capacityHint is queue capacity hint
  /// @return tuple with valid producer and consumer on success
  [[nodiscard]] static auto createProducerAndConsumer(std::string_view name, std::size_t capacityHint) noexcept
      -> std::tuple<Producer, Consumer> {
    try {
      auto const options = turboq::BoundedSPSCRawQueue::CreationOptions{.capacityHint = capacityHint};
      auto queue = turboq::BoundedSPSCRawQueue(name, options, turboq::AnonymousMemorySource());
      return std::make_tuple<Producer, Consumer>(queue.createProducer(), queue.createConsumer());
    } catch (std::exception const& e) {
      fmt::print(stderr, "failed to create producer and consumer: {}\n", e.what());
    }
    return std::make_tuple(Producer(), Consumer());
  }
};

} // namespace fugo::logger
