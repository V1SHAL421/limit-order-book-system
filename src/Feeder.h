#pragma once
#include "SPSCRingBuffer.h"
#include "Wire.h"
#include <chrono>

using std::chrono::nanoseconds;
using std::chrono::steady_clock;
using std::chrono::duration_cast;

template<size_t N = (1u<<15)>
class Feeder {
  SpscRing<OrderCmd, N>& out_;

  static inline TsNanos now_ns() {
    return duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count();
  }

public:
  explicit Feeder(SpscRing<OrderCmd, N>& out) : out_(out) {}

  bool submit(OrderCmd c) {
    c.ts_ns = now_ns();
    return out_.push(c);
  }
};
