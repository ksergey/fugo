// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <turboq/BoundedMPSCRawQueue.h>

#include "Environment.h"

namespace fugo::service {

/// @see turboq::BoundedMPSCRawQueue::CreationOptions;
using CommandQueueCreationOptions = turboq::BoundedMPSCRawQueue::CreationOptions;

struct CommandQueueSender : public turboq::BoundedMPSCRawQueue::Producer {
  CommandQueueSender() = default;

  CommandQueueSender(std::string_view name, Environment const& env)
      : turboq::BoundedMPSCRawQueue::Producer{turboq::BoundedMPSCRawQueue{name, env.memorySource()}.createProducer()} {}

  CommandQueueSender(std::string_view name, CommandQueueCreationOptions const& options, Environment const& env)
      : turboq::BoundedMPSCRawQueue::Producer{
            turboq::BoundedMPSCRawQueue{name, options, env.memorySource()}.createProducer()} {}
};

struct CommandQueueReceiver : public turboq::BoundedMPSCRawQueue::Consumer {
  CommandQueueReceiver(std::string_view name, Environment const& env)
      : turboq::BoundedMPSCRawQueue::Consumer{turboq::BoundedMPSCRawQueue{name, env.memorySource()}.createConsumer()} {}

  CommandQueueReceiver(std::string_view name, CommandQueueCreationOptions const& options, Environment const& env)
      : turboq::BoundedMPSCRawQueue::Consumer{
            turboq::BoundedMPSCRawQueue{name, options, env.memorySource()}.createConsumer()} {}
};

} // namespace fugo::service
