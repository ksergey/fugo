// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <cassert>
#include <optional>

#include <fugo/core/CtString.h>

#include "Validator.h"

namespace fugo::config {
namespace detail {

struct None {};

template <typename T>
struct Value {
  T value;
};

} // namespace detail

template <CtString Name, typename T, typename OptionalT = detail::None, typename ValidatorT = detail::None>
class BasicBinder {
private:
  T* valuePtr_;
  [[no_unique_address]] OptionalT optional_;
  [[no_unique_address]] ValidatorT validator_;

public:
  constexpr BasicBinder(T* valuePtr, OptionalT optional = {}, ValidatorT validator = {}) noexcept
      : valuePtr_{valuePtr}, optional_{optional}, validator_{validator} {
    assert(valuePtr_);
  }

  /// Parameter name
  [[nodiscard]] static constexpr auto name() noexcept -> std::string_view {
    return Name;
  }

  [[nodiscard]] constexpr auto optional(T value) && noexcept -> BasicBinder<Name, T, detail::Value<T>, ValidatorT> {
    return {valuePtr_, detail::Value{value}, std::move(validator_)};
  }

  template <typename ValidatorT2>
    requires Validator<ValidatorT2, T>
  [[nodiscard]] constexpr auto validate(ValidatorT2 validator) && noexcept
      -> BasicBinder<Name, T, OptionalT, ValidatorT2> {
    return {valuePtr_, std::move(optional_), std::move(validator)};
  }

protected:
  [[nodiscard]] constexpr auto valuePtr() const noexcept -> T* {
    return valuePtr_;
  }
};

template <CtString Name, typename T>
[[nodiscard]] constexpr auto value(T* valuePtr) noexcept -> BasicBinder<Name, T> {
  return BasicBinder<Name, T>(valuePtr);
}

} // namespace fugo::config
