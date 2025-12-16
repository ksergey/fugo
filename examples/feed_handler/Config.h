// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <filesystem>
#include <string>

namespace app {

struct ServiceConfig {
  /// Service instance name
  std::string instanceName;
  /// Data queue size hint in Mb (min size 20)
  std::size_t dataQueueSizeHint;
  /// Command queue length hint (min size 1000)
  std::size_t commandQueueLengthHint;
};

struct Config {
  ServiceConfig service;

  Config(std::filesystem::path const& path);
};

} // namespace app
