#include <gtest/gtest.h>
#include "EngineRunner.h"
#include "SPSCRingBuffer.h"
#include "Wire.h"
#include "OrderBookEngine.h"
#include <atomic>
#include <thread>
#include <chrono>

class EngineRunnerTest : public ::testing::Test {
protected:
    void SetUp() override {}

    SpscRing<OrderCmd, 32> ring;
    OrderBookEngine engine;
};

TEST_F(EngineRunnerTest, ProcessSingleOrder) {
    EngineRunner<32> runner(ring, engine);
    OrderCmd cmd{Cmd::New, Side::Buy, OrdType::Limit, 1, 1, 100, 50, 0};
    ring.push(cmd);
    
    std::atomic<bool> stop_flag{false};
    std::thread runner_thread([&]() {
        runner(stop_flag);
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    stop_flag = true;
    runner_thread.join();
    
    EXPECT_TRUE(engine.bestBid().has_value());
    EXPECT_EQ(engine.bestBid().value(), 100);
}

TEST_F(EngineRunnerTest, ProcessMultipleOrders) {
    EngineRunner<32> runner(ring, engine);
    OrderCmd buy_cmd{Cmd::New, Side::Buy, OrdType::Limit, 1, 1, 99, 50, 0};
    OrderCmd sell_cmd{Cmd::New, Side::Sell, OrdType::Limit, 1, 2, 101, 30, 0};
    
    ring.push(buy_cmd);
    ring.push(sell_cmd);
    
    std::atomic<bool> stop_flag{false};
    std::thread runner_thread([&]() {
        runner(stop_flag);
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    stop_flag = true;
    runner_thread.join();
    
    EXPECT_TRUE(engine.bestBid().has_value());
    EXPECT_TRUE(engine.bestAsk().has_value());
    EXPECT_EQ(engine.bestBid().value(), 99);
    EXPECT_EQ(engine.bestAsk().value(), 101);
}

TEST_F(EngineRunnerTest, ProcessMatchingOrders) {
    EngineRunner<32> runner(ring, engine);
    OrderCmd sell_cmd{Cmd::New, Side::Sell, OrdType::Limit, 1, 1, 100, 30, 0};
    OrderCmd buy_cmd{Cmd::New, Side::Buy, OrdType::Limit, 1, 2, 100, 20, 0};
    
    ring.push(sell_cmd);
    ring.push(buy_cmd);
    
    std::atomic<bool> stop_flag{false};
    std::thread runner_thread([&]() {
        runner(stop_flag);
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    stop_flag = true;
    runner_thread.join();
    
    EXPECT_TRUE(engine.bestAsk().has_value());
    EXPECT_EQ(engine.bestAsk().value(), 100);
}

TEST_F(EngineRunnerTest, EmptyQueue) {
    EngineRunner<32> runner(ring, engine);
    std::atomic<bool> stop_flag{false};
    std::thread runner_thread([&]() {
        runner(stop_flag);
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    stop_flag = true;
    runner_thread.join();
    
    EXPECT_FALSE(engine.bestBid().has_value());
    EXPECT_FALSE(engine.bestAsk().has_value());
}

TEST_F(EngineRunnerTest, StopFlag) {
    EngineRunner<32> runner(ring, engine);
    std::atomic<bool> stop_flag{true};
    
    auto start = std::chrono::steady_clock::now();
    runner(stop_flag);
    auto end = std::chrono::steady_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    EXPECT_LT(duration.count(), 10);
}