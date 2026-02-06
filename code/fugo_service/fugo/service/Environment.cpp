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

#include <fugo/core/PosixError.h>

namespace fugo::service {
namespace {

[[nodiscard]] auto getHomePath() noexcept -> std::expected<std::filesystem::path, std::error_code> {
    if (auto const value = std::getenv("HOME"); value != nullptr) {
        return std::filesystem::path{value};
    }
    return std::unexpected(makePosixErrorCode(ENOENT));
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
    if (auto const result = getSystemPath(); result) {
        systemPath_ = result.value();
    } else {
        throw std::system_error{result.error(), "Failed to obtain system path"};
    }

    if (!instanceName_.empty()) {
        dataPath_ = systemPath_ / "data" / instanceName_;
    } else {
        dataPath_ = std::filesystem::current_path();
    }
    std::filesystem::create_directories(dataPath_);
    if (auto const rc = ::chdir(dataPath_.c_str()); rc == -1) {
        throw std::system_error{makePosixErrorCode(errno), "Failed to change current directory"};
    }

    auto memorySourcePath = std::filesystem::path{"/dev/shm"};
    if (!scope_.empty()) {
        memorySourcePath /= scope_;
    }
    std::filesystem::create_directories(memorySourcePath);
    memorySource_ = turboq::DefaultMemorySource{memorySourcePath, std::size_t(::sysconf(_SC_PAGESIZE))};
}

auto Environment::findFile(std::string_view filename) const noexcept
    -> std::expected<std::filesystem::path, std::error_code> {

    // Find file in current dir
    for (std::filesystem::path const& entry : std::filesystem::directory_iterator{std::filesystem::current_path()}) {
        if (entry.filename() == filename) {
            return {entry};
        }
    }

    // Find file in config dir
    auto const configPath = systemPath_ / "config";
    if (is_directory(configPath)) {
        for (std::filesystem::path const& entry : std::filesystem::directory_iterator{configPath}) {
            if (entry.filename() == filename) {
                return {entry};
            }
        }
    }

    return std::unexpected(makePosixErrorCode(ENOENT));
}

auto Environment::getDefaultScope() noexcept -> std::string {
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

} // namespace fugo::service
