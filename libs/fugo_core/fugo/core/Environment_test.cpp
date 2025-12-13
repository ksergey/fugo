// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include <doctest/doctest.h>

#include <print>

#include "Environment.h"

namespace fugo::core {

TEST_CASE("Environment") {
  auto const env = Environment("unit_test");

  std::print("scope: {}\n", env.scope());
  std::print("binaryPath: {}\n", env.binaryPath().c_str());
  std::print("binaryFilename: {}\n", env.binaryPath().filename().c_str());
  std::print("system path: {}\n", env.systemPath().c_str());
  std::print("data path: {}\n", env.dataPath().c_str());

  if (auto const result = env.findConfigFile("xxx"); result) {
    std::print("found(xxx): {}\n", result.value().c_str());
  }
}

} // namespace fugo::core
