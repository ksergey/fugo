// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <fugo/service/Environment.h>
#include <fugo/service/Receiver.h>
#include <fugo/service/ReceiverFeatures.h>
#include <fugo/service/Sender.h>
#include <fugo/service/SenderFeatures.h>

#include "Config.h"

namespace app {

using fugo::service::CommandQueueCreationOptions;
using fugo::service::DataQueueCreationOptions;
using fugo::service::Environment;

struct Sender : public fugo::service::DataQueueSender, public fugo::service::SBESender {
  using fugo::service::DataQueueSender::DataQueueSender;
};

struct Receiver : public fugo::service::CommandQueueReceiver, public fugo::service::SBEReceiver {
  using fugo::service::CommandQueueReceiver::CommandQueueReceiver;
};

class Process {
private:
  Config const& config_;
  Environment const& env_;

  Receiver receiver_;
  Sender sender_;

public:
  Process(Process const&) = delete;
  Process& operator=(Process const&) = delete;

  Process(Config const& config, Environment const& env);
  ~Process();

  /// Start process
  void runInLoop();
};

} // namespace app
