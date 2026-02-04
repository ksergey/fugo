// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <turboq/BoundedMPSCRawQueue.h>

#include "Environment.h"

namespace fugo::service {

/// @see turboq::BoundedMPSCRawQueue::CreationOptions;
using CommandQueueCreationOptions = turboq::BoundedMPSCRawQueue::CreationOptions;

/// Command queue producer
struct CommandQueueProducer : public turboq::BoundedMPSCRawQueue::Producer {
  CommandQueueProducer(CommandQueueProducer&&) = default;
  CommandQueueProducer& operator=(CommandQueueProducer&&) = default;
  CommandQueueProducer() = default;

  CommandQueueProducer(std::string_view name, Environment const& env)
      : turboq::BoundedMPSCRawQueue::Producer{
            turboq::BoundedMPSCRawQueue{std::string{name}.append("@cmd"), env.memorySource()}.createProducer()} {}
};

/// Command queue consumer
struct CommandQueueConsumer : public turboq::BoundedMPSCRawQueue::Consumer {
  CommandQueueConsumer(CommandQueueConsumer&&) = default;
  CommandQueueConsumer& operator=(CommandQueueConsumer&&) = default;
  CommandQueueConsumer() = default;

  CommandQueueConsumer(
      std::string_view name, CommandQueueCreationOptions const& creationOptions, Environment const& env)
      : turboq::BoundedMPSCRawQueue::Consumer{
            turboq::BoundedMPSCRawQueue{std::string{name}.append("@cmd"), creationOptions, env.memorySource()}
                .createConsumer()} {}
};

} // namespace fugo::service
