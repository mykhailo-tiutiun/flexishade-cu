#include "Relay.hpp"
#include "driver/gpio.h"

Relay::Relay(int gpio_num)
    : gpio_num_(gpio_num_t(gpio_num)),
    is_open_(false)
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << gpio_num_);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    gpio_set_level(gpio_num_, 0);
}

void Relay::open() {
    is_open_ = true;
    gpio_set_level(gpio_num_, 1);
}

void Relay::close() {
    is_open_ = false;
    gpio_set_level(gpio_num_, 0);
}

void Relay::toggle() {
    if (is_open_) {
        close();
    } else {
        open();
    }
}

bool Relay::is_open() {
    return is_open_;
}