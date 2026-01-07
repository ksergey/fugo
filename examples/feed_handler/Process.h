// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <fugo/service/BasicCommandReceiver.h>
#include <fugo/service/BasicDataSender.h>
#include <fugo/service/Environment.h>
#include <fugo/service/ReceiverFeatures.h>
#include <fugo/service/SenderFeatures.h>

#include "Config.h"

namespace app {

using fugo::service::Environment;

struct DataSender : public fugo::service::BasicDataSender, public fugo::service::SBESender {
  using fugo::service::BasicDataSender::BasicDataSender;
};

struct CommandReceiver : public fugo::service::BasicCommandReceiver, public fugo::service::SBEReceiver {
  using fugo::service::BasicCommandReceiver::BasicCommandReceiver;
};

class Process {
private:
  Config const& config_;
  Environment const& env_;

  DataSender dataSender_;
  CommandReceiver commandReceiver_;

public:
  Process(Process const&) = delete;
  Process& operator=(Process const&) = delete;

  Process(Config const& config, Environment const& env);
  ~Process();

  /// Start process
  void runInLoop();
};

} // namespace app
