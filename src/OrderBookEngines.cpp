#include "SPSCRingBuffer.h"
#include "Wire.h"
#include "OrderBookEngine.h"
#include "EngineRunner.h"
#include "Feeder.h"
#include <thread>
#include <chrono>
#include <atomic>

using std::thread;
using std::atomic;
using std::this_thread::sleep_for;

int main() {
  SpscRing<OrderCmd, (1u<<15)> q;
  OrderBookEngine engine;
  atomic<bool> stop_flag{false};
  
  thread eng_thr([&q, &engine, &stop_flag]() {
    EngineRunner runner{q, engine};
    runner(stop_flag);
  });
  
  Feeder feeder{ q };

  feeder.submit(OrderCmd{ Cmd::New, Side::Buy, OrdType::Limit, 1, 1, 100, 50, 0 });
  feeder.submit(OrderCmd{ Cmd::New, Side::Sell, OrdType::Limit, 1, 2,  99,  20, 0 });
  feeder.submit(OrderCmd{ Cmd::New, Side::Sell, OrdType::Limit, 1, 3, 100,  40, 0 });

  sleep_for(std::chrono::milliseconds(50));
  stop_flag = true;
  eng_thr.join();
}