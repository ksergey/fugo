// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include <doctest/doctest.h>

#include "QueueManager.h"

namespace fugo::logger {

auto getConsumersCount() noexcept -> std::size_t {
  std::size_t count = 0;
  QueueManager::instance()->forEachConsumer([&]([[maybe_unused]] auto const consumer) {
    count += 1;
  });
  return count;
}

TEST_CASE("QueueManager") {
  REQUIRE_EQ(getConsumersCount(), 0);

  {
    [[maybe_unused]] auto producer = QueueManager::instance()->createProducer(1024 * 1024);
    REQUIRE_EQ(getConsumersCount(), 1);
  }
  REQUIRE_EQ(getConsumersCount(), 1);

  {
    [[maybe_unused]] auto producer = QueueManager::instance()->createProducer();
    REQUIRE_EQ(getConsumersCount(), 2);
  }

  REQUIRE_EQ(getConsumersCount(), 2);

  QueueManager::instance()->forEachConsumer([&](Queue::Consumer* const consumer) {
    consumer->close();
  });

  REQUIRE_EQ(getConsumersCount(), 0);
}

} // namespace fugo::logger
