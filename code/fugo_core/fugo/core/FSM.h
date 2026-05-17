// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <optional>
#include <type_traits>
#include <utility>
#include <variant>

namespace fugo {
namespace core {

#if 0
namespace internal {

template <typename T>
struct is_variant : std::false_type {};

template <typename... Ts>
struct is_variant<std::variant<Ts...>> : std::true_type {};

template <typename T>
inline constexpr auto is_variant_v = is_variant<T>::value;

} // namespace internal
#endif

template <typename... StateTs>
class FSM {
private:
    std::variant<StateTs...> state_;

public:
    constexpr FSM() = default;

    template <typename StateT>
    constexpr FSM(StateT&& state) : state_{std::move(state)} {}

    [[nodiscard]] constexpr auto state() const noexcept -> std::variant<StateTs...> const& {
        return state_;
    }

    [[nodiscard]] constexpr auto state() noexcept -> std::variant<StateTs...>& {
        return state_;
    }

    template <typename SelfT, typename EventT>
    constexpr void process(this SelfT& self, EventT&& event) {
        auto nextState = std::visit(
            [&]<typename StateT>(StateT& state) -> std::optional<std::variant<StateTs...>> {
                return self.onEvent(state, std::forward<EventT>(event));
            },
            self.state_);
        if (nextState) {
            self.state_ = *std::move(nextState);
        }
    }
};

} // namespace core

using namespace ::fugo::core;

} // namespace fugo
