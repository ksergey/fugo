// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <system_error>

#include "Platform.h"

namespace fugo {
namespace core {

class Environment {
private:
  std::string instanceName_;
  std::string scope_;
  std::filesystem::path selfPath_;
  std::filesystem::path systemRootPath_;
  std::filesystem::path dataRootPath_;

public:
  Environment(Environment const&) = default;
  Environment& operator=(Environment const&) = default;

  /// Construct environment
  /// @param[in] instanceName is current instance name
  /// @param[in] scope is environment scope
  /// @throw std::system_error
  Environment(std::string instanceName, std::string scope);

  /// Construct environment with default scope
  Environment(std::string instanceName);

  /// Get instance name
  [[nodiscard]] auto instanceName() const noexcept -> std::string const& {
    return instanceName_;
  }

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

  /// Return path to instance data root path
  [[nodiscard]] auto dataRootPath() const noexcept -> std::filesystem::path const& {
    return dataRootPath_;
  }

  /// Find config file
  /// @param[in] filename is filename
  ///
  /// Find a file in @c systemRootPath / current
  [[nodiscard]] auto findConfigFile(std::string_view filename) const noexcept
      -> std::expected<std::filesystem::path, std::error_code>;
};

} // namespace core

using namespace core;

} // namespace fugo
