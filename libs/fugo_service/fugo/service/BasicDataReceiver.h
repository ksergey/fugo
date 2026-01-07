// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <turboq/BoundedSPMCRawQueue.h>

#include "Environment.h"

namespace fugo::service {

struct BasicDataReceiver : public turboq::BoundedSPMCRawQueue::Consumer {
  BasicDataReceiver(BasicDataReceiver&&) = default;
  BasicDataReceiver& operator=(BasicDataReceiver&&) = default;
  BasicDataReceiver() = default;

  BasicDataReceiver(std::string_view name, Environment const& env)
      : turboq::BoundedSPMCRawQueue::Consumer{
            turboq::BoundedSPMCRawQueue{std::string{name}.append("@data"), env.memorySource()}.createConsumer()} {}
};

} // namespace fugo::service
