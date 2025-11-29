// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#define FUGO_LOG_CALL(LEVEL, FLAGS, FMT, ...)                                                                          \
  do {                                                                                                                 \
    static constexpr auto thisSourceLocation = std::source_location::current();                                        \
    struct {                                                                                                           \
      static constexpr auto location() noexcept -> std::source_location const* {                                       \
        return &thisSourceLocation;                                                                                    \
      }                                                                                                                \
      static constexpr auto level() noexcept -> ::fugo::logger::LogLevel {                                             \
        return ::fugo::logger::LogLevel::LEVEL;                                                                        \
      }                                                                                                                \
      static constexpr auto flags() noexcept -> int {                                                                  \
        return FLAGS;                                                                                                  \
      }                                                                                                                \
      static constexpr auto format() noexcept -> std::string_view {                                                    \
        return std::string_view{FMT};                                                                                  \
      }                                                                                                                \
    } thisMeta;                                                                                                        \
    ::fugo::logger::logStatementTransform<decltype(thisMeta)>(__VA_ARGS__);                                            \
  } while (0)

#define FUGO_LOG(LEVEL, FLAGS, FMT, ...)                                                                               \
  do {                                                                                                                 \
    if (::fugo::logger::LoggerContext::instance()->shouldLog(::fugo::logger::LogLevel::LEVEL)) {                       \
      FUGO_LOG_CALL(LEVEL, FLAGS, FMT, __VA_ARGS__);                                                                   \
    }                                                                                                                  \
  } while (0)

#define logError(...) FUGO_LOG(Error, 0, __VA_ARGS__)
#define logWarning(...) FUGO_LOG(Warning, 0, __VA_ARGS__)
#define logNotice(...) FUGO_LOG(Notice, 0, __VA_ARGS__)
#define logDebug(...) FUGO_LOG(Debug, 0, __VA_ARGS__)
#define logTrace(...) FUGO_LOG(Trace, 0, __VA_ARGS__)

#define logErrorF(...) FUGO_LOG(Error, fugo::logger::kFlagRety, __VA_ARGS__)
#define logWarningF(...) FUGO_LOG(Warning, fugo::logger::kFlagRety, __VA_ARGS__)
#define logNoticeF(...) FUGO_LOG(Notice, fugo::logger::kFlagRety, __VA_ARGS__)
#define logDebugF(...) FUGO_LOG(Debug, fugo::logger::kFlagRety, __VA_ARGS__)
#define logTraceF(...) FUGO_LOG(Trace, fugo::logger::kFlagRety, __VA_ARGS__)
