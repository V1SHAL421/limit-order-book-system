#include "Order.h"
#include <map>
#include "PriceLevel.h"

class OrderBookEngine {
    public:
        bool isLimitOrder(Order order);
        bool isMarketOrder(Order order);
        void addOrder(Order order);
    private:
        std::map<Price, PriceLevel, std::greater<Price>> bids;
        std::map<Price, PriceLevel, std::less<Price>> asks;
};