// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include <doctest/doctest.h>

#include <cstdint>
#include <string>

#include "Json.h"

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

struct ObjectB {
  std::string status;
  std::vector<std::string> events;
  int sourceID;
};

template <typename DTO>
void serialize(ObjectB& obj, DTO& dto) {
  // clang-format off
    dto
      & value<"status">(&obj.status).defaultValue("nothing")
      & value<"events">(&obj.events).defaultValue(std::vector<std::string>{})
      & value<"sourceID">(&obj.sourceID).defaultValue(77)
    ;
  // clang-format on
}

TEST_CASE("Config: JSON 1") {
  ObjectB config;

  {
    auto const result = fromJson(config, R"({
      "status": "draw",
      "events": [ "line", "circle" ]
    })");

    REQUIRE(result);
    REQUIRE_EQ(config.status, "draw");
    REQUIRE_EQ(config.sourceID, 77);

    fmt::print("result:\n{}\n", toJson(config).value());
  }

  {
    auto const result = fromJson(config, R"({
      "status": "draw",
      "events": "dot",
      "sourceID": 133
    })");

    REQUIRE(result);
    REQUIRE_EQ(config.status, "draw");
    REQUIRE_EQ(config.sourceID, 133);

    config.events.push_back("star");

    fmt::print("result:\n{}\n", toJson(config).value());
  }
}

} // namespace fugo::config
