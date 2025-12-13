// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include <doctest/doctest.h>

#include <print>

#include "Environment.h"

namespace fugo::core {

TEST_CASE("Environment") {
  auto const env = Environment("test");
  REQUIRE_EQ(env.instanceName(), "test");
  REQUIRE_EQ(env.scope(), Environment::getDefaultScope());
}

} // namespace fugo::core
