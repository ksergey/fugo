// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include "Environment.h"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <unistd.h>

namespace fugo::core {

Environment::Environment(std::string instanceName, std::string scope)
    : instanceName_{std::move(instanceName)}, scope_{std::move(scope)} {}

Environment::~Environment() {}

auto Environment::findFile(std::string_view filename) -> std::optional<std::filesystem::path> {
  return std::nullopt;
}

auto Environment::getDefaultScope() noexcept -> std::string const& {
  static std::string scope = [] -> std::string {
    char buffer[128];
    if (auto const rc = ::getlogin_r(buffer, sizeof(buffer)); rc != 0) {
      std::fprintf(stderr, "Failed to obtain login: %s\n", ::strerror(errno));
      return {};
    }
    return std::string{buffer};
  }();
  return scope;
}

} // namespace fugo::core
