#ifndef RELAY_HPP
#define RELAY_HPP

#include "soc/gpio_num.h"

class Relay {
    private:
        gpio_num_t gpio_num_;
        bool is_open_;

    public:
        Relay(int gpio_num);
        void open();
        void close();
        void toggle();
        bool is_open();
};

#endif