// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <fugo/service/BroadcastQueue.h>
#include <fugo/service/CommandQueue.h>
#include <fugo/service/Environment.h>

#include "Config.h"

namespace app {

template <typename Features>
struct LocalBroadcastSender {};

template <typename Features>
struct LocalBroadcastReceiver {};

template <typename Features>
struct LocalCommandSender {};

template <typename Features>
struct LocalCommandReceiver {};

using fugo::service::Environment;

class Process {
private:
  Config const& config_;
  Environment const& env_;

public:
  Process(Process const&) = delete;
  Process& operator=(Process const&) = delete;

  Process(Config const& config, Environment const& env);
  ~Process();

  /// Start process
  void runInLoop();
};

} // namespace app
