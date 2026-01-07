// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <turboq/BoundedMPSCRawQueue.h>
#include <turboq/BoundedSPMCRawQueue.h>

#include "Environment.h"

namespace fugo::service {

/// @see turboq::BoundedSPMCRawQueue::CreationOptions;
using DataQueueCreationOptions = turboq::BoundedSPMCRawQueue::CreationOptions;

struct DataQueueReceiver : public turboq::BoundedSPMCRawQueue::Consumer {
  DataQueueReceiver(std::string_view name, Environment const& env)
      : turboq::BoundedSPMCRawQueue::Consumer{
            turboq::BoundedSPMCRawQueue{std::string{name}.append("@data"), env.memorySource()}.createConsumer()} {}

  DataQueueReceiver(std::string_view name, DataQueueCreationOptions const& options, Environment const& env)
      : turboq::BoundedSPMCRawQueue::Consumer{
            turboq::BoundedSPMCRawQueue{std::string{name}.append("@data"), options, env.memorySource()}
                .createConsumer()} {}
};

/// @see turboq::BoundedMPSCRawQueue::CreationOptions;
using CommandQueueCreationOptions = turboq::BoundedMPSCRawQueue::CreationOptions;

struct CommandQueueReceiver : public turboq::BoundedMPSCRawQueue::Consumer {
  CommandQueueReceiver(std::string_view name, Environment const& env)
      : turboq::BoundedMPSCRawQueue::Consumer{
            turboq::BoundedMPSCRawQueue{std::string{name}.append("@command"), env.memorySource()}.createConsumer()} {}

  CommandQueueReceiver(std::string_view name, CommandQueueCreationOptions const& options, Environment const& env)
      : turboq::BoundedMPSCRawQueue::Consumer{
            turboq::BoundedMPSCRawQueue{std::string{name}.append("@command"), options, env.memorySource()}
                .createConsumer()} {}
};

} // namespace fugo::service
