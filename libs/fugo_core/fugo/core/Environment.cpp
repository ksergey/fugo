// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include "Environment.h"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <optional>
#include <unistd.h>

#include <fmt/format.h>

#include "PosixError.h"

namespace fugo::core {
namespace {

[[nodiscard]] auto getDefaultScope() noexcept -> std::string {
  if (auto const value = std::getenv("FUGO_NS"); value != nullptr) {
    return std::string{value};
  }
  char buffer[128];
  if (auto const rc = ::getlogin_r(buffer, sizeof(buffer)); rc == 0) {
    return std::string{buffer};
  } else {
    fmt::print(stderr, "Failed to obtain login: {}\n", ::strerror(errno));
  }
  return {};
}

[[nodiscard]] auto getHomePath() noexcept -> std::expected<std::filesystem::path, std::error_code> {
  if (auto const value = std::getenv("HOME"); value != nullptr) {
    return std::filesystem::path{value};
  }
  return std::unexpected(makePosixErrorCode(ENOENT));
}

[[nodiscard]] auto getBinaryPath() noexcept -> std::expected<std::filesystem::path, std::error_code> {
  char buffer[PATH_MAX];
  if (auto const rc = ::readlink("/proc/self/exe", buffer, sizeof(buffer)); rc != -1) {
    return std::filesystem::path{std::string{buffer, static_cast<std::size_t>(rc)}};
  } else {
    return std::unexpected(makePosixErrorCode(errno));
  }
}

} // namespace

Environment::Environment(std::string scope) : scope_{scope} {
  if (auto rc = getHomePath(); rc) {
    homePath_ = rc.value();
  } else {
    fmt::print(stderr, "Failed to obtain home path: {}\n", rc.error().message());
  }
  if (auto rc = getBinaryPath(); rc) {
    binaryPath_ = rc.value();
  } else {
    fmt::print(stderr, "Failed to obtain this binary path: {}\n", rc.error().message());
  }
}

Environment::Environment() : Environment{getDefaultScope()} {}

auto Environment::findFile(std::string_view filename) -> std::optional<std::filesystem::path> {
  return std::nullopt;
}

} // namespace fugo::core
