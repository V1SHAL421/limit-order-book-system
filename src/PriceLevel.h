#pragma once
#include "Order.h"
#include <deque>

class PriceLevel {
    public:
        void add_order(const Order& o);
        Quantity remove_first(Quantity q);
        bool is_empty();
        Quantity total_quantity() const;
        Quantity reduce_quantity(Quantity q);

    private:
        std::deque<Order> orders;
        Quantity total_quantity_ = 0;
};