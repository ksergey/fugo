// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <concepts>
#include <string>

#include <fugo/core/Result.h>

namespace fugo::config {

using ValidateResult = Result<void, std::string>;

template <typename ValidatorT, typename ArgT>
concept Validator = requires(ValidatorT validator, ArgT arg) {
  { validator(arg) } -> std::same_as<ValidateResult>;
};

/// Checker which do nothing
struct AlwaysPass {
  template <typename T>
  constexpr auto operator()([[maybe_unused]] T const& value) const noexcept -> ValidateResult {
    return {};
  }
};

} // namespace fugo::config
