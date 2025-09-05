#include <cstdint>
#include <map>
#include <deque>
#include <optional>
#include "OrderBookEngine.h"

using std::optional;
using std::nullopt;

bool OrderBookEngine::isLimitOrder(Order order) {
    return order.order_type == OrderType::LIMIT;
}

bool OrderBookEngine::isMarketOrder(Order order) {
    return order.order_type == OrderType::MARKET;
}

void OrderBookEngine::addOrder(Order order) {
    if (order.quantity_remaining <= 0) {
        return;
    };
    if (order.side == Side::BUY) {
        if (!OrderBookEngine::asks.empty()) {
        while (order.quantity_remaining > 0) {
            auto it = OrderBookEngine::asks.begin();
            Price ask_price = it->first;
            PriceLevel& ask_level = it->second;
            if (order.order_type == OrderType::LIMIT && ask_price > order.price) {
                break;
            };

            Quantity order_quantity_satisfied = ask_level.remove_first(order.quantity_remaining);
            order.quantity_remaining -= order_quantity_satisfied;

            if (ask_level.total_quantity() == 0) {
                OrderBookEngine::asks.erase(it);
                continue;
            };
        }
        }
        if (order.quantity_remaining > 0 && order.order_type == OrderType::LIMIT) {
            OrderBookEngine::bids[order.price].add_order(order);
        }
    }
    else {
        if (!OrderBookEngine::bids.empty()) {
        while (order.quantity_remaining > 0) {
            auto it = OrderBookEngine::bids.begin();
            Price bid_price = it->first;
            PriceLevel& bid_level = it->second;
            if (order.order_type == OrderType::LIMIT && bid_price < order.price) {
                break;
            }

            Quantity order_quantity_satisfied = bid_level.remove_first(order.quantity_remaining);
            order.quantity_remaining -= order_quantity_satisfied;

            if (bid_level.total_quantity() == 0) {
                OrderBookEngine::bids.erase(it);
                continue;
            }
        }
        }
        if (order.quantity_remaining > 0 && order.order_type == OrderType::LIMIT) {
            OrderBookEngine::asks[order.price].add_order(order);
        }
    }
}

optional<Price> OrderBookEngine::bestBid() {
    if (OrderBookEngine::bids.empty()) {
        return nullopt;
    }
    return OrderBookEngine::bids.begin()->first;
}

optional<Price> OrderBookEngine::bestAsk() {
    if (OrderBookEngine::asks.empty()) {
        return nullopt;
    }
    return OrderBookEngine::asks.begin()->first;
}