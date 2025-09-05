#include <gtest/gtest.h>
#include "SPSCRingBuffer.h"
#include "Wire.h"
#include "OrderBookEngine.h"
#include "EngineRunner.h"
#include "Feeder.h"
#include <thread>
#include <chrono>
#include <atomic>

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {}

    SpscRing<OrderCmd, 64> ring;
    OrderBookEngine engine;
};

TEST_F(IntegrationTest, FullSystemTest) {
    Feeder<64> feeder{ring};
    EngineRunner<64> runner{ring, engine};
    std::atomic<bool> stop_flag{false};
    
    std::thread runner_thread([&]() {
        runner(stop_flag);
    });
    
    feeder.submit(OrderCmd{Cmd::New, Side::Buy, OrdType::Limit, 1, 1, 99, 100, 0});
    feeder.submit(OrderCmd{Cmd::New, Side::Sell, OrdType::Limit, 1, 2, 101, 50, 0});
    feeder.submit(OrderCmd{Cmd::New, Side::Buy, OrdType::Limit, 1, 3, 100, 75, 0});
    
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    
    stop_flag = true;
    runner_thread.join();
    
    EXPECT_TRUE(engine.bestBid().has_value());
    EXPECT_TRUE(engine.bestAsk().has_value());
}

TEST_F(IntegrationTest, OrderMatching) {
    std::atomic<bool> stop_flag{false};
    
    std::thread runner_thread([&]() {
        runner(stop_flag);
    });
    
    feeder.submit(OrderCmd{Cmd::New, Side::Sell, OrdType::Limit, 1, 1, 100, 50, 0});
    feeder.submit(OrderCmd{Cmd::New, Side::Buy, OrdType::Limit, 1, 2, 100, 30, 0});
    
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    
    stop_flag = true;
    runner_thread.join();
    
    EXPECT_TRUE(engine.bestAsk().has_value());
    EXPECT_EQ(engine.bestAsk().value(), 100);
}

TEST_F(IntegrationTest, HighVolumeTest) {
    std::atomic<bool> stop_flag{false};
    
    std::thread runner_thread([&]() {
        runner(stop_flag);
    });
    
    for (int i = 0; i < 50; ++i) {
        feeder.submit(OrderCmd{Cmd::New, Side::Buy, OrdType::Limit, 1, 
                              static_cast<OrderId>(i), static_cast<Price>(95 + i % 10), 10, 0});
        feeder.submit(OrderCmd{Cmd::New, Side::Sell, OrdType::Limit, 1, 
                              static_cast<OrderId>(i + 1000), static_cast<Price>(105 + i % 10), 10, 0});
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    stop_flag = true;
    runner_thread.join();
    
    EXPECT_TRUE(engine.bestBid().has_value());
    EXPECT_TRUE(engine.bestAsk().has_value());
}

TEST_F(IntegrationTest, MarketOrders) {
    std::atomic<bool> stop_flag{false};
    
    std::thread runner_thread([&]() {
        runner(stop_flag);
    });
    
    feeder.submit(OrderCmd{Cmd::New, Side::Sell, OrdType::Limit, 1, 1, 100, 50, 0});
    feeder.submit(OrderCmd{Cmd::New, Side::Buy, OrdType::Market, 1, 2, 0, 30, 0});
    
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    
    stop_flag = true;
    runner_thread.join();
    
    EXPECT_TRUE(engine.bestAsk().has_value());
}