// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <turboq/BoundedSPMCRawQueue.h>

#include "Environment.h"

namespace fugo::service {

/// @see turboq::BoundedSPMCRawQueue::CreationOptions;
using DataQueueCreationOptions = turboq::BoundedSPMCRawQueue::CreationOptions;

/// Data queue producer
struct DataQueueProducer : public turboq::BoundedSPMCRawQueue::Producer {
  DataQueueProducer(DataQueueProducer&&) = default;
  DataQueueProducer& operator=(DataQueueProducer&&) = default;
  DataQueueProducer() = default;

  DataQueueProducer(std::string_view name, DataQueueCreationOptions const& options, Environment const& env)
      : turboq::BoundedSPMCRawQueue::Producer{
            turboq::BoundedSPMCRawQueue{std::string{name}.append("@data"), options, env.memorySource()}
                .createProducer()} {}
};

/// Data queue consumer
struct DataQueueConsumer : public turboq::BoundedSPMCRawQueue::Consumer {
  DataQueueConsumer(DataQueueConsumer&&) = default;
  DataQueueConsumer& operator=(DataQueueConsumer&&) = default;
  DataQueueConsumer() = default;

  DataQueueConsumer(std::string_view name, Environment const& env)
      : turboq::BoundedSPMCRawQueue::Consumer{
            turboq::BoundedSPMCRawQueue{std::string{name}.append("@data"), env.memorySource()}.createConsumer()} {}
};

} // namespace fugo::service
