// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <filesystem>

#include <fugo/core/Platform.h>
#include <fugo/logger/Logger.h>

namespace fugo::service {

/// @see fugo::logger
using fugo::logger::LogLevel;

/// @see fugo::logger
using fugo::logger::logLevel;

/// @see fugo::logger
using fugo::logger::setLogLevel;

/// @see fugo::logger
using fugo::logger::shouldLog;

struct LoggerOptions {
    /// Bind backend thread to a specified core
    std::optional<std::uint16_t> bindToCoreNo = std::nullopt;
    /// Sleep duration if there is no remaining work to process
    std::chrono::milliseconds sleepDuration = std::chrono::milliseconds{100};
    /// Inter-thread queue capacity hint (Mb)
    std::size_t queueCapacityHint = 4;
    /// Log message pattern
    std::string pattern = "{timestamp} [{level}] {message}";
    /// Log verbosity level (one of error, warning, notice, debug, trace)
    std::string logLevel = "notice";
};

/// Init logger to print messages to stdout
void initLoggerToStdOut(LoggerOptions const& options = {});

/// Init logger to print messages into file at path @c destPath
void initLoggerToFile(std::filesystem::path const& destPath, LoggerOptions const& options = {});

/// Shutdown logger
void shutdownLogger() noexcept;

/// Return true on logger initialized
[[nodiscard]] FUGO_FORCE_INLINE auto isLoggerReady() noexcept {
    return fugo::logger::isBackendReady();
}

} // namespace fugo::service
