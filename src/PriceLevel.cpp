#include "PriceLevel.h"

void PriceLevel::add_order(const Order& o) {
    orders.push_back(o);
    total_quantity_ += o.quantity_remaining;
}

Quantity PriceLevel::remove_first(Quantity order_quantity) {
    if (orders.empty()) {
        return 0;
    }
    else if (order_quantity >= orders.front().quantity_remaining) {
        Quantity best_price_quantity = orders.front().quantity_remaining;
        total_quantity_ -= best_price_quantity;
        orders.pop_front();
        return best_price_quantity;
    }
    else {
        orders.front().quantity_remaining -= order_quantity;
        total_quantity_ -= order_quantity;
        return order_quantity;
    }
};

bool PriceLevel::is_empty() {
    return orders.empty();
}

Quantity PriceLevel::total_quantity() const {
    return total_quantity_;
}

