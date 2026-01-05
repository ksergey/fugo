// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <initializer_list>

#include <fugo/core/Platform.h>

#include "Concepts.h"

namespace fugo::sbe {

template <typename T>
  requires ConstLengthSBEMessage<T>
[[nodiscard]] constexpr auto computeLength() noexcept -> std::size_t {
  using MessageHeader = T::messageHeader;
  return MessageHeader::encodedLength() + T::sbeBlockLength();
}

/// Compute length for message with only 1 repeating group
template <typename T>
  requires NonConstLengthSBEMessage<T>
[[nodiscard]] FUGO_FORCE_INLINE auto computeLength(std::size_t count) noexcept -> std::size_t {
  using MessageHeader = T::messageHeader;
  return MessageHeader::encodedLength() + T::computeLength(count);
}

} // namespace fugo::sbe
