// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <filesystem>
#include <string>

#include "Platform.h"

namespace fugo {
namespace core {

class Environment {
private:
  std::string scope_;
  std::filesystem::path homePath_;
  std::filesystem::path binaryPath_;

public:
  Environment(Environment const&) = default;
  Environment& operator=(Environment const&) = default;

  Environment(std::string scope);

  Environment();

  /// Get environment scope
  [[nodiscard]] auto scope() const noexcept -> std::string const& {
    return scope_;
  }

  /// Return path to home directory
  [[nodiscard]] auto homePath() const noexcept -> std::filesystem::path const& {
    return homePath_;
  }

  /// Return path to current binary
  [[nodiscard]] auto binaryPath() const noexcept -> std::filesystem::path const& {
    return binaryPath_;
  }

  /// Find file in current environment
  [[nodiscard]] auto findFile(std::string_view filename) -> std::optional<std::filesystem::path>;
};

/// Get default environment
[[nodiscard]] FUGO_FORCE_INLINE auto env() -> Environment* {
  static Environment instance;
  return &instance;
}

} // namespace core

using namespace core;

} // namespace fugo
