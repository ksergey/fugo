// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include "Process.h"

#include <fugo/service/Logger.h>

namespace app {

Process::Process(Config const& config, Environment const& env) : config_{config}, env_{env} {
  logNotice("Process created");
}

Process::~Process() {
  logNotice("Process destroyed");
}

void Process::runInLoop() {}

} // namespace app
