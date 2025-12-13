// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace fugo {
namespace core {

class Environment {
private:
  std::string instanceName_;
  std::string scope_;
  std::filesystem::path root_;

public:
  Environment(Environment const&) = default;
  Environment& operator=(Environment const&) = default;
  Environment(Environment&&) = default;
  Environment& operator=(Environment&&) = default;

  Environment(std::string instanceName, std::string scope = getDefaultScope());

  virtual ~Environment();

  /// Get instance name
  [[nodiscard]] auto instanceName() const noexcept -> std::string const& {
    return instanceName_;
  }

  /// Get environment scope
  [[nodiscard]] auto scope() const noexcept -> std::string const& {
    return scope_;
  }

  /// Find file in current environment
  [[nodiscard]] auto findFile(std::string_view filename) -> std::optional<std::filesystem::path>;

  /// Get default scope (or namespace)
  [[nodiscard]] static auto getDefaultScope() noexcept -> std::string const&;
};

} // namespace core

using namespace core;

} // namespace fugo
