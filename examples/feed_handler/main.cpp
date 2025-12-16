// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include <filesystem>

#include <cxxopts.hpp>
#include <fmt/format.h>

#include <fugo/service/Environment.h>

#include "Config.h"

int main(int argc, char* argv[]) {
  try {
    auto options = cxxopts::Options{"FeedHandler", "Demonstration of FeedHandler app"};

    // clang-format off
    options.add_options()
      ("c,config", "Path to config file", cxxopts::value<std::filesystem::path>())
      ("h,help", "Print help and exit")
    ;
    // clang-format on

    auto result = options.parse(argc, argv);
    if (result.count("help")) {
      fmt::print(stdout, "{}\n", options.help());
      return EXIT_FAILURE;
    }

    if (!result.count("config")) {
      throw std::runtime_error{"Missed required option \"--config\""};
    }

    auto const config = app::Config{result["config"].as<std::filesystem::path>()};
    fmt::print(stdout, "instanceName = \"{}\"\n", config.service.instanceName);

  } catch (std::exception const& e) {
    fmt::print(stderr, "ERROR: {}\n", e.what());
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
