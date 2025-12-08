// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include <doctest/doctest.h>

#include <ranges>

#include "Validator.h"

namespace fugo::config {

TEST_CASE("Validator: basic 0") {
  constexpr auto cmp = ge(5) && le(10);
  for (auto i : std::views::iota(0, 4)) {
    REQUIRE_EQ(cmp(i), false);
  }
  for (auto i : std::views::iota(5, 10)) {
    REQUIRE_EQ(cmp(i), true);
  }
  for (auto i : std::views::iota(11, 20)) {
    REQUIRE_EQ(cmp(i), false);
  }
}

TEST_CASE("Validator: basic 1") {
  using namespace std::string_view_literals;

  constexpr auto cmp = eq("hello");
  REQUIRE_EQ(cmp("hello"), true);
  REQUIRE_EQ(cmp("hello"sv), true);
  REQUIRE_EQ(cmp(""), false);
  REQUIRE_EQ(cmp("world"), false);
}

TEST_CASE("Validator: basic 2") {
  using namespace std::string_view_literals;

  constexpr auto cmp0 = empty();
  constexpr auto cmp1 = !empty();
  REQUIRE_EQ(cmp0("hello"), false);
  REQUIRE_EQ(cmp1("hello"), true);
  REQUIRE_EQ(cmp0(""sv), true);
  REQUIRE_EQ(cmp1(""sv), false);
}

} // namespace fugo::config
