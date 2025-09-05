#pragma once
#include "Order.h"
#include <map>
#include <optional>
#include "PriceLevel.h"

class OrderBookEngine {
    public:
        bool isLimitOrder(Order order);
        bool isMarketOrder(Order order);
        void addOrder(Order order);

        std::optional<Price> bestBid();
        std::optional<Price> bestAsk();

    private:
        std::map<Price, PriceLevel, std::greater<Price>> bids;
        std::map<Price, PriceLevel, std::less<Price>> asks;
};