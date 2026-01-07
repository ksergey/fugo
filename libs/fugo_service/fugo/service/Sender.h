// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <turboq/BoundedMPSCRawQueue.h>
#include <turboq/BoundedSPMCRawQueue.h>

#include "Environment.h"

namespace fugo::service {

/// @see turboq::BoundedSPMCRawQueue::CreationOptions;
using DataQueueCreationOptions = turboq::BoundedSPMCRawQueue::CreationOptions;

struct DataQueueSender : public turboq::BoundedSPMCRawQueue::Producer {
  DataQueueSender() = default;

  DataQueueSender(std::string_view name, Environment const& env)
      : turboq::BoundedSPMCRawQueue::Producer{
            turboq::BoundedSPMCRawQueue{std::string{name}.append("@data"), env.memorySource()}.createProducer()} {}

  DataQueueSender(std::string_view name, DataQueueCreationOptions const& options, Environment const& env)
      : turboq::BoundedSPMCRawQueue::Producer{
            turboq::BoundedSPMCRawQueue{std::string{name}.append("@data"), options, env.memorySource()}
                .createProducer()} {}
};

/// @see turboq::BoundedMPSCRawQueue::CreationOptions;
using CommandQueueCreationOptions = turboq::BoundedMPSCRawQueue::CreationOptions;

struct CommandQueueSender : public turboq::BoundedMPSCRawQueue::Producer {
  CommandQueueSender() = default;

  CommandQueueSender(std::string_view name, Environment const& env)
      : turboq::BoundedMPSCRawQueue::Producer{
            turboq::BoundedMPSCRawQueue{std::string{name}.append("@command"), env.memorySource()}.createProducer()} {}

  CommandQueueSender(std::string_view name, CommandQueueCreationOptions const& options, Environment const& env)
      : turboq::BoundedMPSCRawQueue::Producer{
            turboq::BoundedMPSCRawQueue{std::string{name}.append("@command"), options, env.memorySource()}
                .createProducer()} {}
};

} // namespace fugo::service
