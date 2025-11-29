// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include "LoggerContext.h"

#include <cassert>

#include <fmt/format.h>

#include "Codec.h"
#include "StdOutSink.h"

namespace fugo::logger {

LoggerContext::LoggerContext() : sink_(std::make_shared<StdOutSink>()), startOnceFlag_(new std::once_flag) {}

LoggerContext::~LoggerContext() {
  this->stopBackendThread();
  // TODO: ??
  this->backendWork();
}

void LoggerContext::startBackendThread() {
  std::call_once(*startOnceFlag_.load(), [this] {
    this->runBackendThread();

    std::atexit([] {
      loggerContext()->stopBackendThread();
    });
  });
}

void LoggerContext::stopBackendThread() {
  // Stop backend thread
  if (!backendThreadRunning_.exchange(false)) {
    return;
  }

  // Join thread
  if (backendThread_.joinable()) {
    backendThread_.join();
  }

  auto flag = std::make_unique<std::once_flag>();
  auto oldFlag = std::unique_ptr<std::once_flag>(startOnceFlag_.exchange(flag.release()));
}

void LoggerContext::backendWork() {
  // TODO:
  //  Need a buffer for serialization and data and reorder message according to timestamp before write

  auto const sink = sink_.load(std::memory_order_acquire);
  assert(sink.get() != nullptr);

  auto doFlush = false;
  for (auto& queue : threadContextManager_.queues()) {
    auto doCloseQueue = false;
    while (true) {
      auto success = queue.dequeue([&](std::byte const* src) {
        auto const event = Codec<RecordHeader>::decode(src);

        switch (event.type) {
        case EventType::LogRecord: {
          auto const logRecordHeader = Codec<LogRecordHeader>::decode(src);
          auto const metadata = Codec<RecordMetadata*>::decode(src);
          this->processLogRecord(sink.get(), &logRecordHeader, metadata, src);
          doFlush = true;
        } break;
        case EventType::Close: {
          doCloseQueue = true;
        } break;
        default: break;
        }
      });

      // No more messages from queue
      if (!success) {
        break;
      }
    }

    if (doCloseQueue) {
      queue.close();
      threadContextManager_.invalidateQueues();
    }
  }

  if (doFlush) {
    sink->flush();
  }
}

void LoggerContext::processLogRecord(
    Sink* sink, LogRecordHeader const* logRecordHeader, RecordMetadata const* metadata, std::byte const* argsBuffer) {
  assert(sink);
  assert(logRecordHeader);
  assert(metadata);
  assert(argsBuffer);

  // Decode args for format
  fmt::dynamic_format_arg_store<fmt::format_context> store;
  metadata->decodeArgs(argsBuffer, &store);

  formatBuffer_.resize(0);
  fmt::vformat_to(std::back_inserter(formatBuffer_), metadata->format, store);
  auto const message = std::string_view(formatBuffer_.data(), formatBuffer_.size());

  sink->write(*metadata->location, metadata->level, logRecordHeader->timestamp, logRecordHeader->threadID, message);
  sink->flush();
}

void LoggerContext::runBackendThread() {
  auto thread = std::jthread([this] {
    backendThreadRunning_.store(true);

    while (backendThreadRunning_.load(std::memory_order_relaxed)) {
      try {
        this->backendWork();
      } catch (std::exception const& e) {
        fmt::print(stderr, "Logger backend thread error: {}\n", e.what());
      }
      std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }
  });

  try {
    this->backendWork();
  } catch (std::exception const& e) {
    fmt::print(stderr, "Logger backend thread error: {}\n", e.what());
  }

  backendThread_.swap(thread);

  // wait for the thread start
  while (!backendThreadRunning_.load(std::memory_order_seq_cst)) {
    std::this_thread::sleep_for(std::chrono::microseconds{100});
  }
}

} // namespace fugo::logger
