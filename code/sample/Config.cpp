// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include "Config.h"

#include <fugo/config/Config.h>

namespace app {

namespace cfg = fugo::config;

template <typename DTO>
void serialize(ServiceConfig& c, DTO& dto) {
    // clang-format off
    dto
        & cfg::value<"instanceName">(&c.instanceName)
            .validate(!cfg::empty())
        & cfg::value<"dataQueueSizeHint">(&c.dataQueueSizeHint)
            .defaultValue(20)
            .validate(cfg::ge(20))
        & cfg::value<"commandQueueLengthHint">(&c.commandQueueLengthHint)
            .defaultValue(1000)
            .validate(cfg::ge(1000))
    ;
    // clang-format on
}

template <typename DTO>
void serialize(LoggerConfig& c, DTO& dto) {
    using namespace std::string_view_literals;

    // clang-format off
    dto
        & cfg::value<"bindToCoreNo">(&c.bindToCoreNo)
            .defaultValue(std::nullopt)
        & cfg::value<"sleepDuration">(&c.sleepDuration)
            .defaultValue(std::chrono::milliseconds{100})
            .validate(cfg::ge(std::chrono::milliseconds{0}))
        & cfg::value<"queueCapacityHint">(&c.queueCapacityHint)
            .defaultValue(4)
            .validate(cfg::gt(0))
        & cfg::value<"pattern">(&c.pattern)
            .defaultValue("{timestamp} [{level}] {message}")
            .validate(!cfg::empty())
        & cfg::value<"logLevel">(&c.logLevel)
            .defaultValue("notice")
            .validate(cfg::oneOf("error"sv, "warning"sv, "notice"sv, "debug"sv, "trace"sv))
    ;
    // clang-format on
}

template <typename DTO>
void serialize(Config& c, DTO& dto) {
    // clang-format off
    dto
        & cfg::value<"service">(&c.service)
        & cfg::value<"logger">(&c.logger)
            .defaultValue(LoggerConfig{})
    ;
    // clang-format on
}

Config::Config(std::filesystem::path const& path) {
    auto const result = cfg::fromJsonFile(*this, path);
    if (!result) {
        throw std::runtime_error{result.error()};
    }
}

auto Config::toString() const -> std::string {
    return cfg::toJson(*this).value();
}

} // namespace app
