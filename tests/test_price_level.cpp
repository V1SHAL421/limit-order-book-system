#include <gtest/gtest.h>
#include "PriceLevel.h"

class PriceLevelTest : public ::testing::Test {
protected:
    void SetUp() override {
        price_level = PriceLevel();
    }

    PriceLevel price_level;
};

TEST_F(PriceLevelTest, EmptyPriceLevel) {
    EXPECT_TRUE(price_level.is_empty());
    EXPECT_EQ(price_level.total_quantity(), 0);
}

TEST_F(PriceLevelTest, AddSingleOrder) {
    Order order{100, 50, 1, Side::BUY, OrderType::LIMIT};
    price_level.add_order(order);
    
    EXPECT_FALSE(price_level.is_empty());
    EXPECT_EQ(price_level.total_quantity(), 50);
}

TEST_F(PriceLevelTest, AddMultipleOrders) {
    Order order1{100, 30, 1, Side::BUY, OrderType::LIMIT};
    Order order2{100, 20, 2, Side::BUY, OrderType::LIMIT};
    
    price_level.add_order(order1);
    price_level.add_order(order2);
    
    EXPECT_FALSE(price_level.is_empty());
    EXPECT_EQ(price_level.total_quantity(), 50);
}

TEST_F(PriceLevelTest, RemoveFirstFullOrder) {
    Order order{100, 30, 1, Side::BUY, OrderType::LIMIT};
    price_level.add_order(order);
    
    Quantity removed = price_level.remove_first(30);
    
    EXPECT_EQ(removed, 30);
    EXPECT_TRUE(price_level.is_empty());
    EXPECT_EQ(price_level.total_quantity(), 0);
}

TEST_F(PriceLevelTest, RemoveFirstPartialOrder) {
    Order order{100, 50, 1, Side::BUY, OrderType::LIMIT};
    price_level.add_order(order);
    
    Quantity removed = price_level.remove_first(20);
    
    EXPECT_EQ(removed, 20);
    EXPECT_FALSE(price_level.is_empty());
    EXPECT_EQ(price_level.total_quantity(), 30);
}

TEST_F(PriceLevelTest, RemoveFirstMoreThanAvailable) {
    Order order{100, 30, 1, Side::BUY, OrderType::LIMIT};
    price_level.add_order(order);
    
    Quantity removed = price_level.remove_first(50);
    
    EXPECT_EQ(removed, 30);
    EXPECT_TRUE(price_level.is_empty());
    EXPECT_EQ(price_level.total_quantity(), 0);
}

TEST_F(PriceLevelTest, RemoveFromEmptyLevel) {
    Quantity removed = price_level.remove_first(10);
    
    EXPECT_EQ(removed, 0);
    EXPECT_TRUE(price_level.is_empty());
    EXPECT_EQ(price_level.total_quantity(), 0);
}

TEST_F(PriceLevelTest, FIFOOrdering) {
    Order order1{100, 20, 1, Side::BUY, OrderType::LIMIT};
    Order order2{100, 30, 2, Side::BUY, OrderType::LIMIT};
    
    price_level.add_order(order1);
    price_level.add_order(order2);
    
    // Remove first order completely
    Quantity removed1 = price_level.remove_first(20);
    EXPECT_EQ(removed1, 20);
    EXPECT_EQ(price_level.total_quantity(), 30);
    
    // Remove part of second order
    Quantity removed2 = price_level.remove_first(10);
    EXPECT_EQ(removed2, 10);
    EXPECT_EQ(price_level.total_quantity(), 20);
}