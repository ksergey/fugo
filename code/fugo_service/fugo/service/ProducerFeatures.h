// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <bit>
#include <functional>

#include <fugo/sbe/Concepts.h>
#include <fugo/sbe/Utils.h>

namespace fugo::service {

class SBEProducer {
public:
  template <typename MessageT, typename Fn>
    requires fugo::sbe::ConstLengthSBEMessage<MessageT>
  auto send(this auto& impl, Fn&& fn) -> bool {
    constexpr auto encodedMessageSize = fugo::sbe::computeLength<MessageT>();
    return impl.template sendImpl<MessageT>(encodedMessageSize, std::forward<Fn>(fn));
  }

  template <typename MessageT, typename Fn>
    requires fugo::sbe::NonConstLengthSBEMessage<MessageT>
  auto send(this auto& impl, std::size_t groupsCount, Fn&& fn) -> bool {
    auto const encodedMessageSize = fugo::sbe::computeLength<MessageT>(groupsCount);
    return impl.template sendImpl<MessageT>(encodedMessageSize, std::forward<Fn>(fn));
  }

private:
  template <typename MessageT, typename Fn>
  auto sendImpl(this auto& impl, std::size_t encodedMessageSize, Fn&& fn) -> bool {
    auto buffer = impl.prepare(encodedMessageSize);
    if (buffer.empty()) {
      return false;
    }

    // Fill SBE message header and obtain message body
    auto messageBody = MessageT{}.wrapAndApplyHeader(std::bit_cast<char*>(buffer.data()), 0, buffer.size());

    // Fill message body
    std::invoke(std::forward<Fn>(fn), messageBody);

    // Call beforeSend on available
    if constexpr (requires { impl.beforeMessageSend(messageBody); }) {
      impl.beforeMessageSend(messageBody);
    }

    // Commit message
    impl.commit();

    return true;
  }
};

} // namespace fugo::service
