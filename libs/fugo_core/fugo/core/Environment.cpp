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

[[nodiscard]] auto getBinaryPath() noexcept -> std::expected<std::filesystem::path, std::error_code> {
  char buffer[PATH_MAX];
  if (auto const rc = ::readlink("/proc/self/exe", buffer, sizeof(buffer)); rc != -1) {
    return std::filesystem::path{std::string{buffer, static_cast<std::size_t>(rc)}};
  } else {
    return std::unexpected(makePosixErrorCode(errno));
  }
}

[[nodiscard]] auto getSystemPath() noexcept -> std::expected<std::filesystem::path, std::error_code> {
  if (auto const value = std::getenv("FUGO_SYSTEM_PATH"); value != nullptr) {
    return std::filesystem::path{value};
  }
  if (auto const result = getHomePath(); result) {
    return std::filesystem::path{result.value()} / "current";
  }
  return std::unexpected(makePosixErrorCode(ENOENT));
}

} // namespace

Environment::Environment(std::string instanceName, std::string scope)
    : instanceName_{std::move(instanceName)}, scope_{std::move(scope)} {
  if (instanceName_.empty()) {
    throw std::system_error{makePosixErrorCode(EINVAL), "Empty instance name"};
  }
  if (auto result = getBinaryPath(); result) {
    binaryPath_ = result.value();
  } else {
    throw std::system_error{result.error(), "Failed to obtain binary path"};
  }
  if (auto const result = getSystemPath(); result) {
    systemPath_ = result.value();
  } else {
    throw std::system_error{result.error(), "Failed to obtain system path"};
  }

  dataPath_ = systemPath_ / "data" / instanceName_;
  if (!exists(dataPath_)) {
    fmt::print(stdout, "directory {} not exists, creating...\n", dataPath_.c_str());
    // TODO
    // std::create_directories(instanceDataPath_);
  }

#if 0
  if (auto const rc = ::chdir(dataPath_.c_str()); rc == -1) {
    throw std::system_error{makePosixErrorCode(errno), "Failed to change current directory"};
  }
#endif
}

Environment::Environment(std::string instanceName) : Environment{std::move(instanceName), getDefaultScope()} {}

auto Environment::findConfigFile(std::string_view filename) const noexcept
    -> std::expected<std::filesystem::path, std::error_code> {
  auto const configPath = systemPath_ / "config";
  if (!is_directory(configPath)) {
    return std::unexpected(makePosixErrorCode(ENOENT));
  }
  for (std::filesystem::path const& entry : std::filesystem::directory_iterator{configPath}) {
    if (entry.filename() == filename) {
      return {entry};
    }
  }
  return std::unexpected(makePosixErrorCode(ENOENT));
}

} // namespace fugo::core
