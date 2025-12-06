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

// value<"param1">(&param1)
//  .optional(777)
//  .validate(gt(15) && lt(30))

template <CtString Name, typename T, typename OptionalT = detail::None, typename ValidatorT = detail::None>
class ValueBinder {
private:
  T* valuePtr_;
  [[no_unique_address]] OptionalT optional_;
  [[no_unique_address]] ValidatorT validator_;

public:
  ValueBinder(ValueBinder const&) = delete;
  ValueBinder& operator=(ValueBinder const&) = delete;
  ValueBinder(ValueBinder&&) = delete;
  ValueBinder& operator=(ValueBinder&&) = delete;

  constexpr ValueBinder(T* valuePtr, OptionalT optional = {}, ValidatorT validator = {}) noexcept
      : valuePtr_{valuePtr}, optional_{optional}, validator_{validator} {
    assert(valuePtr_);
  }

  /// Parameter name
  [[nodiscard]] static constexpr auto name() noexcept -> std::string_view {
    return Name;
  }

  [[nodiscard]] constexpr auto optional(T value) && noexcept -> ValueBinder<Name, T, detail::Value<T>, ValidatorT> {
    return {valuePtr_, detail::Value{value}, std::move(validator_)};
  }

  template <typename ValidatorT2>
    requires Validator<ValidatorT2, T>
  [[nodiscard]] constexpr auto validate(ValidatorT2 validator) && noexcept
      -> ValueBinder<Name, T, OptionalT, ValidatorT2> {
    return {valuePtr_, std::move(optional_), std::move(validator)};
  }

  [[nodiscard]] constexpr auto valuePtr() const noexcept -> T* {
    return valuePtr_;
  }

  [[nodiscard]] static constexpr auto hasOptional() noexcept -> bool {
    return !std::is_same_v<OptionalT, detail::None>;
  }

  [[nodiscard]] constexpr auto optionalValue() const noexcept -> T const& {
    return optional_.value;
  }

  [[nodiscard]] static constexpr auto hasValidator() noexcept -> bool {
    return !std::is_same_v<ValidatorT, detail::None>;
  }

  [[nodiscard]] constexpr auto validate() const noexcept -> ValidateResult {
    return validator_(*valuePtr());
  }
};

template <CtString Name, typename T>
[[nodiscard]] constexpr auto value(T* valuePtr) noexcept -> ValueBinder<Name, T> {
  return ValueBinder<Name, T>(valuePtr);
}

} // namespace fugo::config
