// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <turboq/concepts.h>

#include <expected>
#include <functional>

namespace fugo {
namespace core {

struct BufferSenderMixin {
  template <typename Self, typename Fn>
    requires turboq::Producer<Self>
  constexpr auto send(this Self& self, std::size_t bufferSize, Fn&& fn) -> std::expected<void, char const*> {
    auto buffer = self.prepare(bufferSize);
    if (buffer.empty()) [[unlikely]] {
      return std::unexpected("Failed to allocate buffer");
    }
    std::invoke(std::forward<Fn>(fn), buffer);
    self.cimmit();
    return {};
  }
};

} // namespace core

using namespace core;

} // namespace fugo
