// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <turboq/platform.h>

/// @see <turboq/platform.h>
#define FUGO_FORCE_INLINE TURBOQ_FORCE_INLINE

/// @see <turboq/platform.h>
#define FUGO_NO_INLINE TURBOQ_NO_INLINE

/// @see <turboq/platform.h>
#define FUGO_COLD TURBOQ_COLD

namespace fugo {
namespace core {

/// @see <turboq/platform.h>
constexpr std::size_t kHardwareDestructiveInterferenceSize = turboq::kHardwareDestructiveInterferenceSize;

/// @see <turboq/platform.h>
constexpr std::size_t kHardwareConstructiveInterferenceSize = turboq::kHardwareConstructiveInterferenceSize;

} // namespace core

using namespace ::fugo::core;

} // namespace fugo
