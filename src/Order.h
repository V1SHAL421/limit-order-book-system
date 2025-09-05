#pragma once
#include <cstdint>

using Price = uint64_t;
using Quantity = int64_t;
using Timestamp = uint64_t;

enum class Side {
    BUY,
    SELL
};

enum class OrderType {
    LIMIT,
    MARKET
};

struct Order {
    Price price;
    Quantity quantity_remaining;
    Timestamp timestamp;
    Side side;
    OrderType order_type;
};