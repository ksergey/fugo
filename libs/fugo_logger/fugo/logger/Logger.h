// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <fmt/format.h>

#include "Codec.h"
#include "LoggerContext.h"
#include "Macro.h"
#include "Transform.h"

namespace fugo::logger {

namespace detail {

template <typename... Args>
constexpr void decodeFormatArgs(
    [[maybe_unused]] std::byte const* src, [[maybe_unused]] fmt::dynamic_format_arg_store<fmt::format_context>* store) {
  (store->push_back(Codec<Args>::decode(src)), ...);
}

} // namespace detail

/// Log statement handler
/// @tparam M is struct with metadata from macro
/// @param[in] args is arguments for log record
template <typename M, typename... Args>
FUGO_FORCE_INLINE void logStatement(Args const&... args) {
  // compile time format string check
  [[maybe_unused]] static constexpr auto formatStringCheck = fmt::format_string<Args...>(M::format());

  auto const now = Clock::now();

  // clang-format off
  [[maybe_unused]] static constexpr auto meta = RecordMetadata{
      .location = M::location(),
      .level = M::level(),
      .format = M::format(),
      .flags = M::flags(),
      .decodeArgs = detail::decodeFormatArgs<Args...>
  };
  // clang-format on

  std::size_t const bufferSize = Codec<RecordHeader>::encodedSize() + Codec<LogRecordHeader>::encodedSize() +
                                 Codec<RecordMetadata const*>::encodedSize(&meta) +
                                 (0 + ... + Codec<Args>::encodedSize(args));

  constexpr auto kEnqueuePolicy = (kFlagRety == (meta.flags & kFlagRety)) ? EnqueuePolicy::Retry : EnqueuePolicy::Drop;

  loggerContext()->currentThreadContext()->producer().enqueue<kEnqueuePolicy>(bufferSize, [&](std::byte* dst) noexcept {
    // RecordHeader
    Codec<RecordHeader>::encode(dst, RecordHeader{.type = EventType::LogRecord});
    // LogRecordHeader
    Codec<LogRecordHeader>::encode(
        dst, LogRecordHeader{.timestamp = now, .threadID = loggerContext()->currentThreadContext()->threadID()});
    // RecordMetadata
    Codec<RecordMetadata const*>::encode(dst, &meta);
    // Args...
    (Codec<Args>::encode(dst, args), ...);
  });
}

/// Transform types into loggable values and pass to log(...)
template <typename M, typename... Args>
FUGO_FORCE_INLINE void logStatementTransform(Args const&... args) {
  return logStatement<M>(transform(args)...);
}

} // namespace fugo::logger
