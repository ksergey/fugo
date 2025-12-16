// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include "Config.h"

#include <fugo/config/Config.h>

namespace app {

namespace cfg = fugo::config;

template <typename DTO>
void serialize(ServiceConfig& c, DTO& dto) {
  // clang-format off
  dto
    & cfg::value<"instanceName">(&c.instanceName)
      .validate(!cfg::empty())
    & cfg::value<"dataQueueSizeHint">(&c.dataQueueSizeHint)
      .defaultValue(20)
      .validate(cfg::ge(20))
    & cfg::value<"commandQueueLengthHint">(&c.commandQueueLengthHint)
      .defaultValue(1000)
      .validate(cfg::ge(1000))
  ;
  // clang-format on
}

template <typename DTO>
void serialize(Config& c, DTO& dto) {
  // clang-format off
  dto
    & cfg::value<"service">(&c.service)
  ;
  // clang-format on
}

Config::Config(std::filesystem::path const& path) {
  auto const result = cfg::fromJsonFile(*this, path);
  if (!result) {
    throw std::runtime_error{result.error()};
  }
}

} // namespace app
