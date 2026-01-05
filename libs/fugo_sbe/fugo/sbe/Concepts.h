// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <concepts>
#include <cstdint>

namespace fugo::sbe {

/// Detect a SBE message fly-weight class
template <typename T>
concept SBEMessage = requires {
  { T::SBE_BLOCK_LENGTH };
  { T::SBE_TEMPLATE_ID };
  { T::SBE_SCHEMA_ID };
  { T::SBE_SCHEMA_VERSION };
  typename T::messageHeader;
};

template <typename T>
concept ConstLengthSBEMessage = SBEMessage<T> && T::isConstLength();

template <typename T>
concept NonConstLengthSBEMessage = SBEMessage<T> && !T::isConstLength();

} // namespace fugo::sbe
