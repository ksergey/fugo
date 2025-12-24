// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include "Process.h"

#include <cassert>
#include <thread>

#include <fugo/core/LoopRateLimit.h>
#include <fugo/core/Signals.h>
#include <fugo/service/Logger.h>

namespace app {

Process::Process(Config const& config, Environment const& env) : config_{config}, env_{env} {
  logNotice("Process created");
}

Process::~Process() {
  logNotice("Process destroyed");
}

void Process::runInLoop() {
  fugo::installSignalHandlers();

  auto loopRateLimit = fugo::LoopRateLimit();

  raise(SIGHUP);

  bool running = true;
  while (running) {
    // Handle process signals (SIGTERM, SIGINT, SIGHUP)
    fugo::notifyCatchedSignals([&](fugo::CatchedSignal const& signal) {
      if (signal.shutdown()) {
        logDebug("Shutdown request");
        running = false;
      }
      if (signal.reload()) {
        logDebug("Reload signal catched");
      }
    });

    loopRateLimit.sleep();
  }

  fugo::restoreSignalHandlers();
}

} // namespace app
