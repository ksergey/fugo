// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <system_error>

#include <fugo/core/Platform.h>

namespace fugo::service {

class Environment {
private:
  std::string instanceName_;
  std::string scope_;
  std::filesystem::path binaryPath_;
  std::filesystem::path systemPath_;
  std::filesystem::path dataPath_;

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
  [[nodiscard]] auto binaryPath() const noexcept -> std::filesystem::path const& {
    return binaryPath_;
  }

  /// Return path to system
  [[nodiscard]] auto systemPath() const noexcept -> std::filesystem::path const& {
    return systemPath_;
  }

  /// Return path to data path
  [[nodiscard]] auto dataPath() const noexcept -> std::filesystem::path const& {
    return dataPath_;
  }

  /// Find config file
  /// @param[in] filename is filename
  [[nodiscard]] auto findConfigFile(std::string_view filename) const noexcept
      -> std::expected<std::filesystem::path, std::error_code>;

  // TODO
  // auto createSender()
  // auto createReceiver()
};

} // namespace fugo::service
