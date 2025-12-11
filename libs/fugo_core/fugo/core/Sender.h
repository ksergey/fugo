// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <functional>

namespace fugo {
namespace core {

template <typename BaseT, template <typename> typename... Fs>
struct BasicSender : public BaseT, Fs<BasicSender<BaseT, Fs...>>... {
  using BaseT::BaseT;

  template <typename Fn>
  constexpr void forEach(Fn&& fn) {
    (std::invoke(std::forward<Fn>(fn), static_cast<Fs<BasicSender<BaseT, Fs...>>&>(*this)), ...);
  }

  template <typename Fn>
  constexpr void forEach(Fn&& fn) const {
    (std::invoke(std::forward<Fn>(fn), static_cast<Fs<BasicSender<BaseT, Fs...>> const&>(*this)), ...);
  }
};

namespace detail {

template <typename BaseT, typename Fs>
struct BasicSenderHelper;
template <typename BaseT, template <typename...> typename List, typename... Fs>
struct BasicSenderHelper<BaseT, List<Fs...>> {
  using type = BasicSender<BaseT, Fs::template Impl...>;
};

} // namespace detail

template <typename...>
struct Features;

template <typename BaseT, typename Fs = Features<>>
using Sender = typename detail::BasicSenderHelper<Base, Fs>::type;

} // namespace core

using namespace core;

} // namespace fugo
