#include "Order.h"
#include <deque>

class PriceLevel {
    public:
        void add_order(const Order& o);
        void remove_first(Quantity q);
        bool is_empty();
        Quantity total_quantity() const;

    private:
        std::deque<Order> orders;
        Quantity total_quantity_ = 0;
};