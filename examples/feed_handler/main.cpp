// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include <filesystem>

#include <cxxopts.hpp>
#include <fmt/format.h>

#include <fugo/service/Environment.h>
#include <fugo/service/Logger.h>

#include "Config.h"
#include "Process.h"

int main(int argc, char* argv[]) {
  using namespace app;

  try {
    auto options = cxxopts::Options{"FeedHandler", "Demonstration of FeedHandler app"};

    // clang-format off
    options.add_options()
      ("c,config", "Path to config file", cxxopts::value<std::filesystem::path>())
      ("p,print", "Print config to stdout and exit")
      ("stdout", "Log messages to stdout (by default log to files)")
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

    auto const config = Config{result["config"].as<std::filesystem::path>()};
    if (result.count("print")) {
      fmt::print(stderr, "{}", config.toString());
      return EXIT_FAILURE;
    }

    // auto const env = fugo::service::Environment{config.service.instanceName};
    auto const env = fugo::service::Environment{{}, {}};

    if (result.count("stdout") > 0) {
      fugo::service::initLoggerToStdOut(config.logger);
    } else {
      fugo::service::initLoggerToFile(env.dataPath(), config.logger);
    }

    logAlwaysF("Config:\n{}", config.toString());

    Process{config, env}.runInLoop();

  } catch (std::exception const& e) {
    if (fugo::service::isLoggerReady()) {
      logErrorF("ERROR: {}", e.what());
    }
    fmt::print(stderr, "ERROR: {}\n", e.what());
    return EXIT_FAILURE;
  }

  fugo::service::shutdownLogger();

  return EXIT_SUCCESS;
}
