// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include <doctest/doctest.h>

#include <cstdint>
#include <string>

#include "Json.h"
#include "ValueBinder.h"

namespace fugo::config {

struct Object {
  std::string path;
  std::int32_t count;

  template <typename DTO>
  void serialize(DTO& dto) {
    // clang-format off
    dto
      & value<"path">(&path)
      & value<"count">(&count).optional(45)
    ;
    // clang-format on
  }
};

TEST_CASE("ValueBinder") {
  int val;

  auto param = value<"param">(&val).optional(777);
  REQUIRE_EQ(param.name(), "param");
}

TEST_CASE("Test") {
  Object result;

  auto json0 = nlohmann::json::parse(R"(
    {"path" : "/dev/shm", "count" : 17}
  )");
  JsonMapping<Object>::read(json0, result);

  REQUIRE_EQ(result.path, "/dev/shm");
  REQUIRE_EQ(result.count, 17);

  auto json1 = nlohmann::json::parse(R"(
    {"path" : "/dev"}
  )");
  JsonMapping<Object>::read(json1, result);

  REQUIRE_EQ(result.path, "/dev");
  REQUIRE_EQ(result.count, 45);

  result.path = "/proc";
  nlohmann::json json;
  JsonMapping<Object>::write(json, result);

  fmt::print("result:{}\n", json.dump(2));
}

} // namespace fugo::config
