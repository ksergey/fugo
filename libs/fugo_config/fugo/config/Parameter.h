// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <fugo/core/CtString.h>

#include "Checker.h"

namespace fugo::config {

/// Metadata for configuration parameter
template <CtString Name, typename T, typename CheckerT = AlwaysPassChecker, typename DefaultT = void>
  requires Checker<CheckerT, T>
struct Parameter {
  /// Parameter name
  static constexpr std::string_view name = Name;
  /// Parameter value type
  using ValueType = T;
  /// Parameter value checker
  using Checker = CheckerT;
  /// Default value provider
  using Default = DefaultT;
};

} // namespace fugo::config
