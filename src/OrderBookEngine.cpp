#include <cstdint>
#include <map>
#include <deque>
#include "OrderBookEngine.h"

bool OrderBookEngine::isLimitOrder(Order order) {
    return order.order_type == OrderType::LIMIT;
}

bool OrderBookEngine::isMarketOrder(Order order) {
    return order.order_type == OrderType::MARKET;
}

// void OrderBookEngine::addLimitOrder(Order order) {
//     if (order.quantity_remaining <= 0) {
//         return;
//     };
//     if (order.side == Side::BUY) {
//         while (order.quantity_remaining > 0) {
//             auto it = OrderBookEngine::asks.begin();
//             Price ask_price = it->first;
//             PriceLevel& ask_level = it->second;
//             if (ask_price > order.price) {
//                 break;
//             };
//             if (ask_level.total_quantity() == 0) {
//                 OrderBookEngine::asks.erase(it);
//                 continue;
//             };

//             }
//         };
//     };