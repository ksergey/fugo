// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include "Parameter.h"

namespace fugo::config {

template <typename ParameterT>
class JsonBinder {
private:
public:
  JsonBinder(JsonBinder const&) = delete;
  JsonBinder& operator=(JsonBinder const&) = delete;
  JsonBinder() = default;
};

} // namespace fugo::config
