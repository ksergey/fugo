// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include <doctest/doctest.h>

#include "BasicBinder.h"

namespace fugo::config {

TEST_CASE("BasicBinder") {
  int val;

  auto param = value<"param">(&val).optional(777);
  REQUIRE_EQ(param.name(), "param");
}

} // namespace fugo::config
