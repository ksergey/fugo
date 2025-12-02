// Copyright (c) Sergey Kovalevich <inndie@gmail.com>
// SPDX-License-Identifier: AGPL-3.0

#include "BackendThread.h"

#include <fmt/format.h>

#include "StdOutSink.h"

namespace fugo::logger {

BackendThread::BackendThread(ThreadContextManager& threadContextManager)
    : threadContextManager_{threadContextManager} {}

BackendThread::~BackendThread() {
  this->stop();
}

void BackendThread::start(std::unique_ptr<Sink> sink) {
  assert(this->isRunning() && "Backend already started");
  assert(sink.get() && "Invalid sink");

  auto thread = std::jthread([this, sink = std::move(sink)] {
    running_.store(true, std::memory_order_seq_cst);
  });

  thread_.swap(thread);

  // Wait until thread started
  while (!running_.load(std::memory_order_seq_cst)) {
    std::this_thread::sleep_for(std::chrono::microseconds(100));

    while (running_.load(std::memory_order_relaxed)) {
      try {
        this->processIncomingLogRecords(*sink);
      } catch (std::exception const& e) {
        fmt::print(stderr, "Logger backend thread error: {}\n", e.what());
      }
      // TODO
      std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }

    while (processIncomingLogRecords(*sink) > 0) {}
  }
}

void BackendThread::stop() {
  // Stop backend thread
  if (!running_.exchange(false)) {
    return;
  }
  // Join thread
  if (thread_.joinable()) {
    thread_.join();
  }
}

auto BackendThread::processIncomingLogRecords(Sink& sink) -> std::size_t {
  std::size_t count = 0;

  auto doFlush = false;
  for (auto& queue : threadContextManager_.queues()) {
    auto doCloseQueue = false;
    while (true) {
      auto success = queue.dequeue([&](std::byte const* src) {
        ++count;
        auto const event = Codec<RecordHeader>::decode(src);

        switch (event.type) {
        case EventType::LogRecord: {
          auto const logRecordHeader = Codec<LogRecordHeader>::decode(src);
          auto const metadata = Codec<RecordMetadata*>::decode(src);
          this->processLogRecord(sink, &logRecordHeader, metadata, src);
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
    sink.flush();
  }

  return count;
}

void BackendThread::processLogRecord(
    Sink& sink, LogRecordHeader const* logRecordHeader, RecordMetadata const* metadata, std::byte const* argsBuffer) {
  assert(logRecordHeader);
  assert(metadata);
  assert(argsBuffer);

  // Decode args for format
  fmt::dynamic_format_arg_store<fmt::format_context> store;
  metadata->decodeArgs(argsBuffer, &store);

  formatBuffer_.resize(0);
  fmt::vformat_to(std::back_inserter(formatBuffer_), metadata->format, store);
  auto const message = std::string_view(formatBuffer_.data(), formatBuffer_.size());

  sink.write(*metadata->location, metadata->level, logRecordHeader->timestamp, logRecordHeader->threadID, message);
}

} // namespace fugo::logger
