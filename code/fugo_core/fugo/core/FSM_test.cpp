// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include <doctest/doctest.h>

#include <print>

#include "FSM.h"

namespace fugo::core {

struct StateA {};
struct StateB {};
struct StateC {};

// clang-format off
using Variant = std::variant<
    StateA,
    StateB,
    StateC
>;
// clang-format on

template <int>
struct Event {};

struct SampleFSM : FSM<StateA, StateB, StateC> {
    using FSM<StateA, StateB, StateC>::FSM;

    template <typename StateT, typename EventT>
    auto onEvent(StateT, EventT) {
        std::print("uhnalded event for state\n");
        return std::nullopt;
    }

    auto onEvent(StateA, Event<0>) {
        return StateB{};
    }

    auto onEvent(StateB, Event<1>) {
        return StateC{};
    }

    auto onEvent(StateB, Event<2>) {
        return StateA{};
    }

    auto onEvent(StateB, Event<3>) {
        return std::nullopt;
    }
};

TEST_CASE("SampleFSM") {
    SampleFSM fsm{StateA{}};

    std::visit(
        []<typename StateT>(StateT const&) {
            REQUIRE(std::is_same_v<StateT, StateA>);
        },
        fsm.state());

    fsm.process(Event<0>{});

    std::visit(
        []<typename StateT>(StateT const&) {
            REQUIRE(std::is_same_v<StateT, StateB>);
        },
        fsm.state());

    fsm.process(Event<0>{});

    std::visit(
        []<typename StateT>(StateT const&) {
            REQUIRE(std::is_same_v<StateT, StateB>);
        },
        fsm.state());

    fsm.process(Event<1>{});

    std::visit(
        []<typename StateT>(StateT const&) {
            REQUIRE(std::is_same_v<StateT, StateC>);
        },
        fsm.state());
}

} // namespace fugo::core
