// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include <doctest/doctest.h>

#include "ThreadContextManager.h"

namespace fugo::logger {

TEST_CASE("ThreadContextManager") {
  ThreadContextManager manager;
  REQUIRE(manager.queues().empty());

  static thread_local auto context = manager.createThreadContext();
  REQUIRE(!context.producer().closed());

  REQUIRE_EQ(manager.queues().size(), 1);
  REQUIRE(!manager.queues()[0].closed());
}

} // namespace fugo::logger
