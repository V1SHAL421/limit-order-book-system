#include "Order.h"
#include <map>
#include "PriceLevel.h"

class OrderBookEngine {
    public:
        bool isLimitOrder(Order order);
        bool isMarketOrder(Order order);

        std::optional<Price> bestBid();
        std::optional<Price> bestAsk();

    private:
        void addOrder(Order order);
        std::map<Price, PriceLevel, std::greater<Price>> bids;
        std::map<Price, PriceLevel, std::less<Price>> asks;
};