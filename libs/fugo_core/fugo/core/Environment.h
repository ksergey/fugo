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
  std::filesystem::path selfPath_;
  std::filesystem::path systemRootPath_;

public:
  Environment(Environment const&) = default;
  Environment& operator=(Environment const&) = default;

  /// Construct environment
  /// @param[in] scope is environment scope
  Environment(std::string scope);

  /// Construct environment with default scope
  Environment();

  /// Get environment scope
  [[nodiscard]] auto scope() const noexcept -> std::string const& {
    return scope_;
  }

  /// Return path to self binary (obtained from /proc/self/exe)
  [[nodiscard]] auto selfPath() const noexcept -> std::filesystem::path const& {
    return selfPath_;
  }

  /// Return path to system root
  [[nodiscard]] auto systemRootPath() const noexcept -> std::filesystem::path const& {
    return systemRootPath_;
  }

  // TODO: DATA path
  // TODO: set current path (::chdir) (and create)

  /// Find config file in config dir
  [[nodiscard]] auto findConfigFile(std::string_view filename) -> std::optional<std::filesystem::path>;
};

/// Get default environment
[[nodiscard]] FUGO_FORCE_INLINE auto env() -> Environment* {
  static Environment instance;
  return &instance;
}

} // namespace core

using namespace core;

} // namespace fugo
