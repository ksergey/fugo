// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <fugo/sbe/Concepts.h>
#include <fugo/sbe/Utils.h>

#include "sbe_local/GetInstrumentList.h"
#include "sbe_local/Heartbeat.h"
#include "sbe_local/InstrumentList.h"
#include "sbe_local/ServiceDown.h"
#include "sbe_local/ServiceUp.h"

namespace app {

template <typename... ArgsT>
    requires(fugo::sbe::SBEMessage<ArgsT> && ...)
struct List;

// using AdminMessages = List<sbe_local::Heartbeat, sbe_local::ServiceUp, sbe_local::ServiceDown>;

using RequestMessages = List<sbe_local::GetInstrumentList>;

} // namespace app
