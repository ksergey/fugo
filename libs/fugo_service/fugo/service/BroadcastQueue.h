// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <turboq/BoundedSPMCRawQueue.h>

#include "Environment.h"

namespace fugo::service {

/// @see turboq::BoundedSPMCRawQueue::CreationOptions;
using BroadcastQueueCreationOptions = turboq::BoundedSPMCRawQueue::CreationOptions;

struct BroadcastQueueSender : public turboq::BoundedSPMCRawQueue::Producer {
  BroadcastQueueSender() = default;

  BroadcastQueueSender(std::string_view name, Environment const& env)
      : turboq::BoundedSPMCRawQueue::Producer{turboq::BoundedSPMCRawQueue{name, env.memorySource()}.createProducer()} {}

  BroadcastQueueSender(std::string_view name, BroadcastQueueCreationOptions const& options, Environment const& env)
      : turboq::BoundedSPMCRawQueue::Producer{
            turboq::BoundedSPMCRawQueue{name, options, env.memorySource()}.createProducer()} {}
};

struct BroadcastQueueReceiver : public turboq::BoundedSPMCRawQueue::Consumer {
  BroadcastQueueReceiver(std::string_view name, Environment const& env)
      : turboq::BoundedSPMCRawQueue::Consumer{turboq::BoundedSPMCRawQueue{name, env.memorySource()}.createConsumer()} {}

  BroadcastQueueReceiver(std::string_view name, BroadcastQueueCreationOptions const& options, Environment const& env)
      : turboq::BoundedSPMCRawQueue::Consumer{
            turboq::BoundedSPMCRawQueue{name, options, env.memorySource()}.createConsumer()} {}
};

} // namespace fugo::service
