#include <gtest/gtest.h>
#include "OrderBookEngine.h"

class OrderBookEngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        engine = OrderBookEngine();
    }

    OrderBookEngine engine;
};

TEST_F(OrderBookEngineTest, IsLimitOrder) {
    Order limit_order{100, 50, 1, Side::BUY, OrderType::LIMIT};
    EXPECT_TRUE(engine.isLimitOrder(limit_order));
    
    Order market_order{0, 50, 2, Side::BUY, OrderType::MARKET};
    EXPECT_FALSE(engine.isLimitOrder(market_order));
}

TEST_F(OrderBookEngineTest, IsMarketOrder) {
    Order market_order{0, 50, 1, Side::BUY, OrderType::MARKET};
    EXPECT_TRUE(engine.isMarketOrder(market_order));
    
    Order limit_order{100, 50, 2, Side::BUY, OrderType::LIMIT};
    EXPECT_FALSE(engine.isMarketOrder(limit_order));
}

TEST_F(OrderBookEngineTest, BestBidAskEmpty) {
    EXPECT_FALSE(engine.bestBid().has_value());
    EXPECT_FALSE(engine.bestAsk().has_value());
}

TEST_F(OrderBookEngineTest, AddLimitBuyOrder) {
    Order buy_order{100, 50, 1, Side::BUY, OrderType::LIMIT};
    engine.addOrder(buy_order);
    
    EXPECT_TRUE(engine.bestBid().has_value());
    EXPECT_EQ(engine.bestBid().value(), 100);
}

TEST_F(OrderBookEngineTest, AddLimitSellOrder) {
    Order sell_order{110, 50, 1, Side::SELL, OrderType::LIMIT};
    engine.addOrder(sell_order);
    
    EXPECT_TRUE(engine.bestAsk().has_value());
    EXPECT_EQ(engine.bestAsk().value(), 110);
}

TEST_F(OrderBookEngineTest, MatchingOrders) {
    // Add sell order first
    Order sell_order{100, 30, 1, Side::SELL, OrderType::LIMIT};
    engine.addOrder(sell_order);
    EXPECT_TRUE(engine.bestAsk().has_value());
    
    // Add buy order that should match
    Order buy_order{100, 20, 2, Side::BUY, OrderType::LIMIT};
    engine.addOrder(buy_order);
    
    // Ask should still exist with remaining quantity
    EXPECT_TRUE(engine.bestAsk().has_value());
    EXPECT_EQ(engine.bestAsk().value(), 100);
}