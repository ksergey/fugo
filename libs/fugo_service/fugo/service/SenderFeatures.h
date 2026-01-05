// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <bit>
#include <functional>

#include <fugo/sbe/Concepts.h>
#include <fugo/sbe/Prepare.h>

namespace fugo::service {

struct SBESender {
  template <typename MessageT, typename Fn>
    requires fugo::sbe::ConstLengthSBEMessage<MessageT>
  auto send(this auto& impl, Fn&& fn) -> bool {
    constexpr auto encodedMessageSize = fugo::sbe::computeLength<MessageT>();
    auto buffer = impl.prepare(encodedMessageSize);
    if (buffer.empty()) {
      return false;
    }
    std::invoke(
        std::forward<Fn>(fn), MessageT{}.wrapAndApplyHeader(std::bit_cast<char*>(buffer.data()), 0, buffer.size()));
    impl.commit();
    return true;
  }

  template <typename MessageT, typename Fn>
    requires fugo::sbe::NonConstLengthSBEMessage<MessageT>
  auto send(this auto& impl, std::size_t groupsCount, Fn&& fn) -> bool {
    auto const encodedMessageSize = fugo::sbe::computeLength<MessageT>(groupsCount);
    auto buffer = impl.prepare(encodedMessageSize);
    if (buffer.empty()) {
      return false;
    }
    std::invoke(
        std::forward<Fn>(fn), MessageT{}.wrapAndApplyHeader(std::bit_cast<char*>(buffer.data()), 0, buffer.size()));
    impl.commit();
    return true;
  }
};

} // namespace fugo::service
