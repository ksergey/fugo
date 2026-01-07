// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <turboq/BoundedSPMCRawQueue.h>

#include "Environment.h"

namespace fugo::service {

/// @see turboq::BoundedSPMCRawQueue::CreationOptions;
using BasicDataSenderCreationOptions = turboq::BoundedSPMCRawQueue::CreationOptions;

struct BasicDataSender : public turboq::BoundedSPMCRawQueue::Producer {
  BasicDataSender(BasicDataSender&&) = default;
  BasicDataSender& operator=(BasicDataSender&&) = default;
  BasicDataSender() = default;

  BasicDataSender(std::string_view name, BasicDataSenderCreationOptions const& options, Environment const& env)
      : turboq::BoundedSPMCRawQueue::Producer{
            turboq::BoundedSPMCRawQueue{std::string{name}.append("@data"), options, env.memorySource()}
                .createProducer()} {}
};

} // namespace fugo::service
