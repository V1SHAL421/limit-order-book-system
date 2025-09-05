#include <gtest/gtest.h>
#include "SPSCRingBuffer.h"
#include "Wire.h"

class SPSCRingTest : public ::testing::Test {
protected:
    void SetUp() override {}

    SpscRing<int, 8> ring;
};

TEST_F(SPSCRingTest, EmptyRing) {
    EXPECT_TRUE(ring.empty());
    
    int value;
    EXPECT_FALSE(ring.pop(value));
}

TEST_F(SPSCRingTest, PushSingleItem) {
    EXPECT_TRUE(ring.push(42));
    EXPECT_FALSE(ring.empty());
}

TEST_F(SPSCRingTest, PushPopSingleItem) {
    EXPECT_TRUE(ring.push(42));
    
    int value;
    EXPECT_TRUE(ring.pop(value));
    EXPECT_EQ(value, 42);
    EXPECT_TRUE(ring.empty());
}

TEST_F(SPSCRingTest, PushMultipleItems) {
    for (int i = 0; i < 7; ++i) {
        EXPECT_TRUE(ring.push(i));
    }
    EXPECT_FALSE(ring.empty());
}

TEST_F(SPSCRingTest, RingFull) {
    for (int i = 0; i < 7; ++i) {
        EXPECT_TRUE(ring.push(i));
    }
    EXPECT_FALSE(ring.push(999));
}

TEST_F(SPSCRingTest, FIFOOrdering) {
    for (int i = 0; i < 5; ++i) {
        EXPECT_TRUE(ring.push(i));
    }
    
    for (int i = 0; i < 5; ++i) {
        int value;
        EXPECT_TRUE(ring.pop(value));
        EXPECT_EQ(value, i);
    }
}

TEST_F(SPSCRingTest, WrapAround) {
    for (int i = 0; i < 7; ++i) {
        EXPECT_TRUE(ring.push(i));
    }
    
    for (int i = 0; i < 3; ++i) {
        int value;
        EXPECT_TRUE(ring.pop(value));
        EXPECT_EQ(value, i);
    }
    
    for (int i = 100; i < 103; ++i) {
        EXPECT_TRUE(ring.push(i));
    }
    
    for (int i = 3; i < 7; ++i) {
        int value;
        EXPECT_TRUE(ring.pop(value));
        EXPECT_EQ(value, i);
    }
    
    for (int i = 100; i < 103; ++i) {
        int value;
        EXPECT_TRUE(ring.pop(value));
        EXPECT_EQ(value, i);
    }
}