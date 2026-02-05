// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include <string>

#include <cxxopts.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include <fugo/core/LoopRateLimit.h>
#include <fugo/service/ConsumerFeatures.h>
#include <fugo/service/DataQueue.h>
#include <fugo/service/Environment.h>

#include "Messaging.h"

struct DataConsumer : public fugo::service::DataQueueConsumer, fugo::service::SBEConsumer {
  using fugo::service::DataQueueConsumer::DataQueueConsumer;
};

template <typename...>
struct MessageList;

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
  try {
    auto options = cxxopts::Options{"dump", "dump sbe messages from data queue"};

    // clang-format off
    options.add_options()
      ("n,instance-name", "Data queue instance name", cxxopts::value<std::string>())
      ("s,scope", "Environment scope", cxxopts::value<std::string>()->default_value(""))
      ("h,help", "Print help and exit")
    ;
    // clang-format on

    auto result = options.parse(argc, argv);
    if (result.count("help")) {
      fmt::print(stdout, "{}\n", options.help());
      return EXIT_FAILURE;
    }

    if (!result.count("instance-name")) {
      throw std::runtime_error{"Missed required option \"--instance-name\""};
    }
    auto const& instanceName = result["instance-name"].as<std::string>();

    auto const env = fugo::service::Environment{{}, result["scope"].as<std::string>()};

    auto loopRateLimit = fugo::LoopRateLimit();

    auto dataReceiver = DataConsumer{instanceName, env};

    using Messages = MessageList<sbe_local::Heartbeat, sbe_local::ServiceUp, sbe_local::ServiceDown>;

    while (true) {
      dataReceiver.receive<Messages>([](auto sbeBody) {
        fmt::print(stdout, "{}\n", fmt::streamed(sbeBody));
      });
      loopRateLimit.sleep();
    }

  } catch (std::exception const& e) {
    fmt::print(stderr, "ERROR: {}\n", e.what());
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
