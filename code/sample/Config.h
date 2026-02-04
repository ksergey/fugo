// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <filesystem>
#include <string>

#include <fugo/service/Logger.h>

namespace app {

struct ServiceConfig {
  /// Service instance name
  std::string instanceName;
  /// Data queue size hint in Mb (min size 20)
  std::size_t dataQueueSizeHint;
  /// Command queue length hint (min size 1000)
  std::size_t commandQueueLengthHint;
};

struct LoggerConfig : fugo::service::LoggerOptions {};

struct Config {
  ServiceConfig service;
  LoggerConfig logger;

  /// Load config from json file
  Config(std::filesystem::path const& path);

  /// Serialize to string
  [[nodiscard]] auto toString() const -> std::string;
};

} // namespace app
