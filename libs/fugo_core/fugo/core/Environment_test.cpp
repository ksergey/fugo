// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include <doctest/doctest.h>

#include <print>

#include "Environment.h"

namespace fugo::core {

TEST_CASE("Environment") {
  std::print("scope: {}\n", env()->scope());
  std::print("homePath: {}\n", env()->homePath().c_str());
  std::print("binaryPath: {}\n", env()->binaryPath().c_str());
  std::print("filename: {}\n", env()->binaryPath().filename().c_str());
}

} // namespace fugo::core
