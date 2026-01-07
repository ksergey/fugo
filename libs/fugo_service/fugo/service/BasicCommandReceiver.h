// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <turboq/BoundedMPSCRawQueue.h>

#include "Environment.h"

namespace fugo::service {

/// @see turboq::BoundedMPSCRawQueue::CreationOptions;
using BasicCommandReceiverCreationOptions = turboq::BoundedMPSCRawQueue::CreationOptions;

struct BasicCommandReceiver : public turboq::BoundedMPSCRawQueue::Consumer {
  BasicCommandReceiver(BasicCommandReceiver&&) = default;
  BasicCommandReceiver& operator=(BasicCommandReceiver&&) = default;
  BasicCommandReceiver() = default;

  BasicCommandReceiver(
      std::string_view name, BasicCommandReceiverCreationOptions const& creationOptions, Environment const& env)
      : turboq::BoundedMPSCRawQueue::Consumer{
            turboq::BoundedMPSCRawQueue{std::string{name}.append("@cmd"), creationOptions, env.memorySource()}
                .createConsumer()} {}
};

} // namespace fugo::service
