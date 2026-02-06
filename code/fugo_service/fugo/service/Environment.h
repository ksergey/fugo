// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <system_error>

#include <turboq/MemorySource.h>

#include <fugo/core/Platform.h>

namespace fugo::service {

class Environment {
  private:
    std::string instanceName_;
    std::string scope_;
    std::filesystem::path systemPath_;
    std::filesystem::path dataPath_;
    turboq::DefaultMemorySource memorySource_;

  public:
    Environment(Environment const&) = default;
    Environment& operator=(Environment const&) = default;

    /// Construct environment
    /// @param[in] instanceName is current instance name
    /// @param[in] scope is environment scope
    /// @throw std::system_error
    Environment(std::string instanceName = {}, std::string scope = getDefaultScope());

    /// Get instance name
    [[nodiscard]] auto instanceName() const noexcept -> std::string const& {
        return instanceName_;
    }

    /// Get environment scope
    [[nodiscard]] auto scope() const noexcept -> std::string const& {
        return scope_;
    }

    /// Return path to system
    [[nodiscard]] auto systemPath() const noexcept -> std::filesystem::path const& {
        return systemPath_;
    }

    /// Return path to data path
    [[nodiscard]] auto dataPath() const noexcept -> std::filesystem::path const& {
        return dataPath_;
    }

    /// Return MemorySource for turboq queues
    [[nodiscard]] auto memorySource() const noexcept -> turboq::MemorySource const& {
        return memorySource_;
    }

    /// Find config file
    /// @param[in] filename is filename
    [[nodiscard]] auto findFile(std::string_view filename) const noexcept
        -> std::expected<std::filesystem::path, std::error_code>;

    /// Get default scope
    [[nodiscard]] static auto getDefaultScope() noexcept -> std::string;
};

} // namespace fugo::service
