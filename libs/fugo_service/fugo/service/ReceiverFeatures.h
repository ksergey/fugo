// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <bit>
#include <functional>

#include <fugo/sbe/Concepts.h>

namespace fugo::service {

struct SBEReceiver {
  // TODO: check ListT contains sbe messages with the same MessageHeader
  template <typename ListT, typename Fn>
  auto receive(this auto& impl, Fn&& fn) -> bool {
    bool receivedAtLeastOne = false;
    while (true) {
      auto const buffer = impl.fetch();
      if (buffer.empty()) {
        break;
      }
      receivedAtLeastOne = true;
      // TODO: decode and invoke
      impl.consume();
    }
    return receivedAtLeastOne;
  }
};

} // namespace fugo::service
