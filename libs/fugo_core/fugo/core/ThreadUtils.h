// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <cstdint>
#include <string>

#include "Result.h"

namespace fugo {
namespace core {

/// Return name of the thread
auto getThisThreadName() -> Result<std::string>;

/// Set name of the thread
auto setThisThreadName(std::string const& value) -> Result<>;

/// Pin current thread to specified core number
auto pinCurrentThreadToCoreNo(std::uint16_t coreNo) noexcept -> Result<>;

} // namespace core

using namespace ::fugo::core;

} // namespace fugo
