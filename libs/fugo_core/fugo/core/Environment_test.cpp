// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include <doctest/doctest.h>

#include <print>

#include "Environment.h"

namespace fugo::core {

TEST_CASE("Environment") {
  std::print("scope: {}\n", env()->scope());
  std::print("selfPath: {}\n", env()->selfPath().c_str());
  std::print("selfFilename: {}\n", env()->selfPath().filename().c_str());
  std::print("systemRoot: {}\n", env()->systemRootPath().c_str());

  if (auto const result = env()->findConfigFile("xxx"); result) {
    std::print("found(xxx): {}\n", result.value().c_str());
  }
}

} // namespace fugo::core
