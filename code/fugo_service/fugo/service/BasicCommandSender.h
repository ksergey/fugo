// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <turboq/BoundedMPSCRawQueue.h>

#include "Environment.h"

namespace fugo::service {

struct BasicCommandSender : public turboq::BoundedMPSCRawQueue::Producer {
  BasicCommandSender(BasicCommandSender&&) = default;
  BasicCommandSender& operator=(BasicCommandSender&&) = default;
  BasicCommandSender() = default;

  BasicCommandSender(std::string_view name, Environment const& env)
      : turboq::BoundedMPSCRawQueue::Producer{
            turboq::BoundedMPSCRawQueue{std::string{name}.append("@cmd"), env.memorySource()}.createProducer()} {}
};

} // namespace fugo::service
