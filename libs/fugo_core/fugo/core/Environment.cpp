// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include "Environment.h"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <optional>
#include <ranges>
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

[[nodiscard]] auto getSelfPath() noexcept -> std::expected<std::filesystem::path, std::error_code> {
  char buffer[PATH_MAX];
  if (auto const rc = ::readlink("/proc/self/exe", buffer, sizeof(buffer)); rc != -1) {
    return std::filesystem::path{std::string{buffer, static_cast<std::size_t>(rc)}};
  } else {
    return std::unexpected(makePosixErrorCode(errno));
  }
}

[[nodiscard]] auto getSystemRoot() noexcept -> std::expected<std::filesystem::path, std::error_code> {
  if (auto const value = std::getenv("FUGO_SYSTEM_ROOT"); value != nullptr) {
    return std::filesystem::path{value};
  }
  if (auto const result = getHomePath(); result) {
    return std::filesystem::path{result.value()} / "current";
  }
  return std::unexpected(makePosixErrorCode(ENOENT));
}

} // namespace

Environment::Environment(std::string scope) : scope_{scope} {
  if (auto result = getSelfPath(); result) {
    selfPath_ = result.value();
  } else {
    fmt::print(stderr, "Failed to obtain self binary path: {}\n", result.error().message());
  }
  if (auto const result = getSystemRoot(); result) {
    systemRootPath_ = result.value();
  } else {
    fmt::print(stderr, "Failed to obtain system root path: {}\n", result.error().message());
  }
}

Environment::Environment() : Environment{getDefaultScope()} {}

auto Environment::findConfigFile(std::string_view filename) -> std::optional<std::filesystem::path> {
  auto const configPath = systemRootPath_ / "config";
  if (!is_directory(configPath)) {
    return std::nullopt;
  }
  for (std::filesystem::path const& entry : std::filesystem::directory_iterator{configPath}) {
    if (entry.filename() == filename) {
      return entry;
    }
  }
  return std::nullopt;
}

} // namespace fugo::core
