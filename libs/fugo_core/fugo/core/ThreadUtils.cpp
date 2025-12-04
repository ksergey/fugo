// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include "ThreadUtils.h"

#include <cerrno>

#include <pthread.h>

namespace fugo {
namespace core {

auto getThisThreadName() -> Result<std::string> {
  constexpr auto kThreadNameMaxLength = std::size_t(16);
  char name[kThreadNameMaxLength] = {'\0'};
  auto const rc = ::pthread_getname_np(::pthread_self(), name, kThreadNameMaxLength);
  if (rc != 0) {
    return {makePosixErrorCode(errno)};
  }
  return {std::string(name)};
}

auto setThisThreadName(std::string const& value) -> Result<> {
  auto const rc = ::pthread_setname_np(::pthread_self(), value.c_str());
  if (rc != 0) {
    return {makePosixErrorCode(errno)};
  }
  return success();
}

auto pinCurrentThreadToCoreNo(std::uint16_t coreNo) noexcept -> Result<> {
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(coreNo, &cpuset);

  if (auto const rc = ::pthread_setaffinity_np(::pthread_self(), sizeof(cpuset), &cpuset); rc != 0) {
    return {makePosixErrorCode(errno)};
  }
  return success();
}

} // namespace core
} // namespace fugo
