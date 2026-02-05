// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <functional>

#include <fugo/core/Platform.h>

#include "Concepts.h"

namespace fugo::sbe {

template <typename T>
  requires ConstLengthSBEMessage<T>
[[nodiscard]] constexpr auto computeLength() noexcept -> std::size_t {
  using MessageHeader = T::messageHeader;
  return MessageHeader::encodedLength() + T::sbeBlockLength();
}

/// Compute length for message with only 1 repeating group
template <typename T>
  requires NonConstLengthSBEMessage<T>
[[nodiscard]] FUGO_FORCE_INLINE auto computeLength(std::size_t count) noexcept -> std::size_t {
  using MessageHeader = T::messageHeader;
  return MessageHeader::encodedLength() + T::computeLength(count);
}

template <typename MessageT, typename MessageHeaderT>
  requires SBEMessage<MessageT> && std::is_same_v<MessageHeaderT, typename MessageT::messageHeader>
constexpr auto sbeCast(MessageHeaderT sbeMessageHeader) noexcept -> MessageT {
  return MessageT{sbeMessageHeader.buffer(), sbeMessageHeader.encodedLength(), sbeMessageHeader.bufferLength(),
      sbeMessageHeader.blockLength(), sbeMessageHeader.version()};
}

namespace detail {

template <typename ListT>
struct TakeFirst;
template <template <typename...> typename List, typename T, typename... Ts>
struct TakeFirst<List<T, Ts...>> {
  using Type = T;
};

template <typename ListT>
struct SBEMessageHeaderImpl;
template <template <typename...> typename List, typename... Ts>
  requires((SBEMessage<Ts> && ... && true))
struct SBEMessageHeaderImpl<List<Ts...>> {
  using Type = typename TakeFirst<List<typename Ts::messageHeader...>>::Type;
  static_assert((std::is_same_v<Type, typename Ts::messageHeader> && ... && true));
};

} // namespace detail

template <typename ListT>
using SBEMessageHeader = typename detail::SBEMessageHeaderImpl<ListT>::Type;

namespace detail {

template <typename ListT>
struct SBEVisitImpl;
template <template <typename...> typename List, typename... Ts>
  requires((SBEMessage<Ts> && ... && true))
struct SBEVisitImpl<List<Ts...>> {
  template <typename MessageHeaderT, typename Fn>
  static void invoke(MessageHeaderT const& sbeMessageHeader, Fn&& fn) {
    static_assert((std::is_same_v<MessageHeaderT, typename Ts::messageHeader> && ... && true));

    (((sbeMessageHeader.templateId() == Ts::SBE_TEMPLATE_ID)
             ? (std::invoke(std::forward<Fn>(fn), sbeCast<Ts>(sbeMessageHeader)), true)
             : false) ||
        ... || false);
  }
};

} // namespace detail

template <typename ListT, typename MessageHeaderT, typename Fn>
constexpr void sbeVisit(MessageHeaderT sbeMessageHeader, Fn&& fn) {
  return detail::SBEVisitImpl<ListT>::invoke(sbeMessageHeader, std::forward<Fn>(fn));
}

} // namespace fugo::sbe
