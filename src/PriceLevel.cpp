#include "PriceLevel.h"

void PriceLevel::add_order(const Order& o) {
    orders.push_back(o);
    total_quantity_ += o.quantity_remaining;
}

void PriceLevel::remove_first(Quantity order_quantity) {
    if (orders.empty()) {
    }
    else if (order_quantity >= orders.front().quantity_remaining) {
        total_quantity_ -= orders.front().quantity_remaining;
        orders.pop_front();
    }
    else {
        orders.front().quantity_remaining -= order_quantity;
    }
    return;
};

bool PriceLevel::is_empty() {
    return orders.empty();
}