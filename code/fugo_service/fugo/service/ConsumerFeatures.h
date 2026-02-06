// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <bit>
#include <functional>

#include <fugo/sbe/Concepts.h>
#include <fugo/sbe/Utils.h>

namespace fugo::service {

struct SBEConsumer {
    template <typename ListT, typename Fn>
    auto receive(this auto& impl, Fn&& fn) -> bool {
        using MessageHeader = fugo::sbe::SBEMessageHeader<ListT>;

        bool receivedAtLeastOne = false;
        while (true) {
            auto const buffer = impl.fetch();
            if (buffer.empty()) {
                break;
            }
            receivedAtLeastOne = true;

            auto const messageHeader =
                MessageHeader{const_cast<char*>(std::bit_cast<char const*>(buffer.data())), buffer.size()};
            fugo::sbe::sbeVisit<ListT>(messageHeader, std::forward<Fn>(fn));
            impl.consume();
        }
        return receivedAtLeastOne;
    }
};

} // namespace fugo::service
