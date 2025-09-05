#include <gtest/gtest.h>
#include "Wire.h"

class WireTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(WireTest, CmdEnum) {
    EXPECT_EQ(static_cast<uint8_t>(Cmd::New), 0);
    EXPECT_EQ(static_cast<uint8_t>(Cmd::Cancel), 1);
}

TEST_F(WireTest, SideEnum) {
    EXPECT_EQ(static_cast<uint8_t>(Side::Buy), 0);
    EXPECT_EQ(static_cast<uint8_t>(Side::Sell), 1);
}

TEST_F(WireTest, OrdTypeEnum) {
    EXPECT_EQ(static_cast<uint8_t>(OrdType::Limit), 0);
    EXPECT_EQ(static_cast<uint8_t>(OrdType::Market), 1);
}

TEST_F(WireTest, OrderCmdConstruction) {
    OrderCmd cmd{
        Cmd::New,
        Side::Buy,
        OrdType::Limit,
        1,
        12345,
        100,
        50,
        1000000
    };
    
    EXPECT_EQ(cmd.cmd, Cmd::New);
    EXPECT_EQ(cmd.side, Side::Buy);
    EXPECT_EQ(cmd.type, OrdType::Limit);
    EXPECT_EQ(cmd.sym, 1);
    EXPECT_EQ(cmd.id, 12345);
    EXPECT_EQ(cmd.px, 100);
    EXPECT_EQ(cmd.qty, 50);
    EXPECT_EQ(cmd.ts_ns, 1000000);
}

TEST_F(WireTest, OrderCmdSize) {
    EXPECT_EQ(sizeof(OrderCmd), 40);
}

TEST_F(WireTest, TypeAliases) {
    SymbolId sym = 1;
    OrderId id = 12345;
    Price px = 100;
    Qty qty = 50;
    TsNanos ts = 1000000;
    
    EXPECT_EQ(sizeof(sym), 2);
    EXPECT_EQ(sizeof(id), 8);
    EXPECT_EQ(sizeof(px), 8);
    EXPECT_EQ(sizeof(qty), 8);
    EXPECT_EQ(sizeof(ts), 8);
}