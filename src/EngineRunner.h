#pragma once
#include "SPSCRingBuffer.h"
#include "Wire.h"
#include "OrderBookEngine.h"
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>

using std::atomic;
using std::vector;
using std::thread;
using std::this_thread::yield;
using std::chrono::nanoseconds;
using std::chrono::steady_clock;
using std::chrono::duration_cast;

template <size_t QSZ = (1u << 15)>
class EngineRunner {
  SpscRing<OrderCmd, QSZ>& in_;
  OrderBookEngine& engine_;
  static constexpr int kBatch = 256;

  static inline TsNanos now_ns() {
    return duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count();
  }

public:
  EngineRunner(SpscRing<OrderCmd, QSZ>& in, OrderBookEngine& eng)
    : in_(in), engine_(eng) {}

  void operator()(atomic<bool>& stop_flag) {
    vector<OrderCmd> batch;
    batch.reserve(kBatch);

    while (!stop_flag.load()) {
      batch.clear();
      OrderCmd c;
      for (int i = 0; i < kBatch; ++i) {
        if (!in_.pop(c)) break;
        batch.push_back(c);
      }
      if (batch.empty()) {
        yield();
        continue;
      }

      for (const auto& cmd : batch) {
        if (cmd.cmd == Cmd::New) {
          Order o {
            cmd.px,
            cmd.qty,
            now_ns(),
            cmd.side == Side::Buy ? Side::Buy : Side::Sell,
            cmd.type == OrdType::Limit ? OrderType::LIMIT : OrderType::MARKET
          };
          engine_.addOrder(o);
        } else {
        }
      }
    }
  }
};