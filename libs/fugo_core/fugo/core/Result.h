// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <turboq/Result.h>

namespace fugo {
namespace core {

/// @see <turboq/Result.h>
using turboq::Result;

/// @see <turboq/Result.h>
using turboq::success;

/// @see <turboq/Result.h>
using turboq::makePosixErrorCode;

/// @see <turboq/Result.h>
using turboq::getPosixErrorCategory;

} // namespace core

using namespace ::fugo::core;

} // namespace fugo
