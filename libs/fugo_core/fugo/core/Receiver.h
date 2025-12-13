// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

namespace fugo {
namespace core {

template <typename ImplT, template <typename> typename... Fs>
struct BasicReceiver {};

} // namespace core

using namespace core;

} // namespace fugo
