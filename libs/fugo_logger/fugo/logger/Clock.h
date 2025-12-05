// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <cstdint>
#include <ctime>

#include <fugo/core/Platform.h>

namespace fugo::logger {

/// Clock for timestamps
template <clockid_t ClockID>
struct LinuxClock {
  using Timestamp = ::timespec;

  [[nodiscard]] FUGO_FORCE_INLINE static auto now() noexcept -> Timestamp {
    timespec ts;
    ::clock_gettime(ClockID, &ts);
    return ts;
  }

  [[nodiscard]] static constexpr auto toTimeSpec(::timespec const& value) noexcept -> ::timespec const& {
    return value;
  }
};

struct TSCClock {
  using Timestamp = std::int64_t;

  [[nodiscard]] FUGO_FORCE_INLINE static auto now() noexcept -> Timestamp {
    return __builtin_ia32_rdtsc();
  }

  [[nodiscard]] static auto toTimeSpec(std::int64_t value) noexcept -> ::timespec;
};

// using Clock = LinuxClock<CLOCK_REALTIME>;
using Clock = TSCClock;

} // namespace fugo::logger
