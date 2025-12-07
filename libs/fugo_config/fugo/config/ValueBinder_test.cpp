// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include <doctest/doctest.h>

#include <cstdint>
#include <string>

#include "Json.h"
#include "ValueBinder.h"

namespace fugo::config {

struct ObjectA {
  std::string path;
  std::int32_t count;

  template <typename DTO>
  void serialize(DTO& dto) {
    // clang-format off
    dto
      & value<"path">(&path)
      & value<"count">(&count).defaultValue(45)
    ;
    // clang-format on
  }
};

TEST_CASE("Config: JSON 0") {
  ObjectA result;

  auto json0 = nlohmann::json::parse(R"(
    {"path" : "/dev/shm", "count" : 17}
  )");
  JsonMapping<ObjectA>::read(json0, result);

  REQUIRE_EQ(result.path, "/dev/shm");
  REQUIRE_EQ(result.count, 17);

  auto json1 = nlohmann::json::parse(R"(
    {"path" : "/dev"}
  )");
  JsonMapping<ObjectA>::read(json1, result);

  REQUIRE_EQ(result.path, "/dev");
  REQUIRE_EQ(result.count, 45);

  result.path = "/proc";
  nlohmann::json json;
  JsonMapping<ObjectA>::write(json, result);

  fmt::print("result:{}\n", json.dump(2));
}

TEST_CASE("Config: JSON 1") {

}

} // namespace fugo::config
