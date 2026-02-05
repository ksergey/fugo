// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#pragma once

#include <chrono>
#include <cstdint>

#include <fugo/service/CommandQueue.h>
#include <fugo/service/ConsumerFeatures.h>
#include <fugo/service/DataQueue.h>
#include <fugo/service/Environment.h>
#include <fugo/service/Logger.h>
#include <fugo/service/ProducerFeatures.h>

#include "Config.h"
#include "Messaging.h"

namespace app {

constexpr auto kServiceIDNullValue = sbe_local::Heartbeat::serviceIDNullValue();
constexpr auto kApplicationIDNullValue = sbe_local::GetInstrumentList::applicationIDNullValue();
constexpr auto kRequestIDNullValue = sbe_local::GetInstrumentList::requestIDNullValue();

using fugo::service::Environment;

struct CommandConsumer : public fugo::service::CommandQueueConsumer, public fugo::service::SBEConsumer {
  using fugo::service::CommandQueueConsumer::CommandQueueConsumer;
};

class ServiceFeature {
private:
  std::uint32_t serviceID_ = kServiceIDNullValue;
  std::uint32_t applicationID_ = kApplicationIDNullValue;

public:
  [[nodiscard]] auto serviceID() const noexcept -> std::uint32_t {
    return serviceID_;
  }
  void setServiceID(std::uint32_t value) noexcept {
    serviceID_ = value;
  }
  [[nodiscard]] auto applicationID() const noexcept -> std::uint32_t {
    return applicationID_;
  }
  void setApplicationID(std::uint32_t value) noexcept {
    applicationID_ = value;
  }
};

template <typename Clock = std::chrono::steady_clock>
class HeartbeatFeature {
private:
  Clock::time_point deadline_ = {};
  std::chrono::milliseconds heartbeatInterval_ = std::chrono::seconds{1};

public:
  [[nodiscard]] auto heartbeatInterval() const noexcept -> std::chrono::milliseconds {
    return heartbeatInterval_;
  }
  void setHeartbeatInterval(std::chrono::milliseconds value) noexcept {
    heartbeatInterval_ = value;
  }
  void keepAlive(this auto& impl) {
    auto const now = Clock::now();
    if (now < impl.deadline_) [[likely]] {
      return;
    }
    impl.template send<sbe_local::Heartbeat>([&](auto body) {
      body.serviceID(impl.serviceID());
    });
    impl.deadline_ = now + impl.heartbeatInterval_;
  }
  void serviceUp(this auto& impl) {
    impl.template send<sbe_local::ServiceUp>([&](auto body) {
      body.serviceID(impl.serviceID());
    });
  }
  void serviceDown(this auto& impl) {
    impl.template send<sbe_local::ServiceDown>([&](auto body) {
      body.serviceID(impl.serviceID());
    });
  }
};

struct DataProducer : fugo::service::DataQueueProducer, fugo::service::SBEProducer, ServiceFeature, HeartbeatFeature<> {
  using fugo::service::DataQueueProducer::DataQueueProducer;
};

class Process {
private:
  Config const& config_;
  Environment const& env_;

  DataProducer dataProducer_;
  CommandConsumer commandConsumer_;

public:
  Process(Process const&) = delete;
  Process& operator=(Process const&) = delete;

  Process(Config const& config, Environment const& env);
  ~Process();

  /// Start process
  void runInLoop();
};

} // namespace app
