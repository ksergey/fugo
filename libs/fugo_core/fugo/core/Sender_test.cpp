// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include <doctest/doctest.h>

#include "Sender.h"

namespace fugo::core {

struct SPSCQueueSender : BufferSenderMixin {};

TEST_CASE("Sender Test") {
  // SPSCQueueSender sender;
  // sender.send(125, [](std::span<std::byte> data) {});
}

} // namespace fugo::core
