// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <concepts>
#include <string>

#include <fugo/core/Result.h>

namespace fugo::config {

using CheckResult = Result<void, std::string>;

template <typename CheckerT, typename ArgT>
concept Checker = requires(CheckerT checker, ArgT arg) {
  { checker(arg) } -> std::same_as<CheckResult>;
};

/// Checker which do nothing
struct AlwaysPassChecker {
  template <typename T>
  constexpr auto operator()([[maybe_unused]] T const& value) const noexcept -> CheckResult {
    return {};
  }
};

} // namespace fugo::config
