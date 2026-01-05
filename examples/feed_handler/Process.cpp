// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include "Process.h"

#include <cassert>
#include <thread>

#include <fugo/core/LoopRateLimit.h>
#include <fugo/core/Signals.h>
#include <fugo/service/Logger.h>

namespace app {
namespace {

constexpr auto kMaxCommandLengthHint = std::size_t(100); // TODO:

} // namespace

Process::Process(Config const& config, Environment const& env)
    : config_{config}, env_{env},
      receiver_{config_.service.instanceName,
          CommandQueueCreationOptions{kMaxCommandLengthHint, config_.service.commandQueueLengthHint}, env_},
      sender_{
          config_.service.instanceName, DataQueueCreationOptions{config_.service.dataQueueSizeHint * (1 << 20)}, env_} {

  logNotice("Environment");
  logNotice("  instanceName = \"{}\"/\"{}\"", env_.instanceName(), config_.service.instanceName);
  logNotice("  scope = \"{}\"", env_.scope());
  logNotice("  systemPath = \"{}\"", env_.systemPath());
  logNotice("  dataPath = \"{}\"", env_.dataPath());

  logNotice("Process created");
}

Process::~Process() {
  logNotice("Process destroyed");
}

void Process::runInLoop() {
  fugo::installSignalHandlers();

  auto loopRateLimit = fugo::LoopRateLimit();

  bool running = true;
  while (running) {
    // Handle process signals (SIGTERM, SIGINT, SIGHUP)
    fugo::notifyCatchedSignals([&](fugo::CatchedSignal const& signal) {
      // SIGTERM, SIGINT
      if (signal.shutdown()) {
        logDebug("Shutdown request");
        running = false;
      }
      // SIGHUP
      if (signal.reload()) {
        logDebug("Reload signal catched");
      }
    });

    loopRateLimit.sleep();
  }

  fugo::restoreSignalHandlers();
}

} // namespace app
