// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <string>
#include <type_traits>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include "ValueBinder.h"

namespace fugo::config {

struct JsonException : std::runtime_error {
  template <typename... Args>
  JsonException(fmt::format_string<Args...> fmtStr, Args&&... args)
      : std::runtime_error{fmt::format(fmtStr, std::forward<Args>(args)...)} {}
};

template <typename T>
struct JsonMapping;

template <typename ValueBinderT>
struct JsonBinder {
  ValueBinderT& binder;

  void from(nlohmann::json const& json) const {
    using ValueT = std::remove_pointer_t<decltype(binder.valuePtr())>;

    if (auto const found = json.find(binder.name()); found != json.end()) {
      JsonMapping<ValueT>::read(found.value(), *binder.valuePtr());
    } else {
      if constexpr (ValueBinderT::hasOptional()) {
        *binder.valuePtr() = binder.optionalValue();
      } else {
        throw JsonException("Value \"{}\" not found", binder.name());
      }
    }

    if constexpr (ValueBinderT::hasValidator()) {
      if (auto const result = binder.validate(); !result) {
        throw JsonException("Invalid value \"{}\" ({})", binder.name(), result.error());
      }
    }
  }

  void to(nlohmann::json& json) {
    using ValueT = std::remove_pointer_t<decltype(binder.valuePtr())>;

    if constexpr (ValueBinderT::hasValidator()) {
      if (auto const result = binder.validate(); !result) {
        throw JsonException("Invalid value \"{}\" ({})", binder.name(), result.error());
      }
    }

    nlohmann::json entry;
    JsonMapping<ValueT>::write(entry, *binder.valuePtr());
    json[binder.name()] = entry;
  }
};

namespace detail {

class JsonInput {
private:
  nlohmann::json const& json_;

public:
  JsonInput(nlohmann::json const& json) noexcept : json_{json} {}

  /// operator& for serialize(...).
  template <typename ValueBinderT>
  auto operator&(ValueBinderT const& binder) -> JsonInput& {
    JsonBinder{binder}.from(json_);
    return *this;
  }
};

/// Helper for map `operator&` to `Binder::to`.
class JsonOutput {
private:
  nlohmann::json& json_;

public:
  JsonOutput(nlohmann::json& json) noexcept : json_{json} {}

  /// operator& for serialize(...).
  template <typename ValueBinderT>
  auto operator&(ValueBinderT const& binder) -> JsonOutput& {
    JsonBinder{binder}.to(json_);
    return *this;
  }
};

} // namespace detail

template <typename T>
concept SerializeMember = requires(T& a, detail::JsonInput& input, detail::JsonOutput& output) {
  { a.serialize(input) };
  { a.serialize(output) };
};

template <typename T>
concept SerializeFreeFunction = requires(T& a, detail::JsonInput& input, detail::JsonOutput& output) {
  { serialize(a, input) };
  { serialize(a, output) };
};

/// Mapping for T::serialize(DTO&)
template <typename T>
  requires SerializeMember<T>
struct JsonMapping<T> {
  static void read(nlohmann::json const& json, T& value) {
    detail::JsonInput input{json};
    value.serialize(input);
  }
  static void write(nlohmann::json& json, T const& value) {
    detail::JsonOutput output{json};
    const_cast<T&>(value).serialize(output);
  }
};

/// Mapping for serialize(T&, DTO&)
template <typename T>
  requires SerializeFreeFunction<T>
struct JsonMapping<T> {
  static void read(nlohmann::json const& json, T& value) {
    detail::JsonInput input{json};
    serialize(value, input);
  }
  static void write(nlohmann::json& json, T const& value) {
    detail::JsonOutput output{json};
    serialize(const_cast<T&>(value), output);
  }
};

template <typename T>
  requires std::is_arithmetic_v<T>
struct JsonMapping<T> {
  static void read(nlohmann::json const& json, T& value) {
    value = json.get<T>();
  }
  static void write(nlohmann::json& json, T const& value) {
    json = value;
  }
};

template <>
struct JsonMapping<std::string> {
  static void read(nlohmann::json const& json, std::string& value) {
    value = json.get<std::string>();
  }
  static void write(nlohmann::json& json, std::string const& value) {
    json = value;
  }
};

} // namespace fugo::config
