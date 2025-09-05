#include <gtest/gtest.h>
#include "Feeder.h"
#include "SPSCRingBuffer.h"
#include "Wire.h"

class FeederTest : public ::testing::Test {
protected:
    void SetUp() override {}

    SpscRing<OrderCmd, 16> ring;
    Feeder<16> feeder{ring};
};

TEST_F(FeederTest, SubmitOrder) {
    OrderCmd cmd{Cmd::New, Side::Buy, OrdType::Limit, 1, 1, 100, 50, 0};
    
    EXPECT_TRUE(feeder.submit(cmd));
    EXPECT_FALSE(ring.empty());
}

TEST_F(FeederTest, SubmitMultipleOrders) {
    for (int i = 0; i < 10; ++i) {
        OrderCmd cmd{Cmd::New, Side::Buy, OrdType::Limit, 1, static_cast<OrderId>(i), 100, 50, 0};
        EXPECT_TRUE(feeder.submit(cmd));
    }
}

TEST_F(FeederTest, TimestampSet) {
    OrderCmd cmd{Cmd::New, Side::Buy, OrdType::Limit, 1, 1, 100, 50, 0};
    
    EXPECT_TRUE(feeder.submit(cmd));
    
    OrderCmd retrieved;
    EXPECT_TRUE(ring.pop(retrieved));
    EXPECT_GT(retrieved.ts_ns, 0);
}

TEST_F(FeederTest, OrderPreserved) {
    OrderCmd original{Cmd::New, Side::Sell, OrdType::Market, 2, 999, 200, 75, 0};
    
    EXPECT_TRUE(feeder.submit(original));
    
    OrderCmd retrieved;
    EXPECT_TRUE(ring.pop(retrieved));
    
    EXPECT_EQ(retrieved.cmd, original.cmd);
    EXPECT_EQ(retrieved.side, original.side);
    EXPECT_EQ(retrieved.type, original.type);
    EXPECT_EQ(retrieved.sym, original.sym);
    EXPECT_EQ(retrieved.id, original.id);
    EXPECT_EQ(retrieved.px, original.px);
    EXPECT_EQ(retrieved.qty, original.qty);
}

TEST_F(FeederTest, RingFull) {
    for (int i = 0; i < 15; ++i) {
        OrderCmd cmd{Cmd::New, Side::Buy, OrdType::Limit, 1, static_cast<OrderId>(i), 100, 50, 0};
        EXPECT_TRUE(feeder.submit(cmd));
    }
    
    OrderCmd cmd{Cmd::New, Side::Buy, OrdType::Limit, 1, 999, 100, 50, 0};
    EXPECT_FALSE(feeder.submit(cmd));
}