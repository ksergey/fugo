// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <fugo/service/CommandQueue.h>
#include <fugo/service/DataQueue.h>
#include <fugo/service/Environment.h>
#include <fugo/service/SenderFeatures.h>

#include "Config.h"

namespace app {

using fugo::service::CommandQueueCreationOptions;
using fugo::service::CommandQueueReceiver;
using fugo::service::DataQueueCreationOptions;
using fugo::service::Environment;

struct DataQueueSender : public fugo::service::DataQueueSender, fugo::service::SBESender {
  using fugo::service::DataQueueSender::DataQueueSender;
};

class Process {
private:
  Config const& config_;
  Environment const& env_;

  CommandQueueReceiver receiver_;
  DataQueueSender sender_;

public:
  Process(Process const&) = delete;
  Process& operator=(Process const&) = delete;

  Process(Config const& config, Environment const& env);
  ~Process();

  /// Start process
  void runInLoop();
};

} // namespace app
