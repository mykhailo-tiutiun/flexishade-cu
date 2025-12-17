#include "relay/Relay.hpp"
#include "driver/gpio.h"
#include <cstdio>
#include <optional>
#include <string>

Relay::Relay(LocalId local_id, int gpio_num)
    : local_id_(local_id),
    gpio_num_((gpio_num_t)gpio_num),
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

void Relay::open()
{
    is_open_ = true;
    printf("relay is on\n");
    gpio_set_level(gpio_num_, 1);
}

void Relay::close()
{
    is_open_ = false;
    printf("relay is off\n");
    gpio_set_level(gpio_num_, 0);
}

void Relay::toggle()
{
    if (is_open_) {
        close();
    } else {
        open();
    }
}

const LocalId& Relay::getLocalId() const
{
    return local_id_;
}

const std::optional<GlobalId>& Relay::getGlobalId() const
{
    return global_id_;
}

bool Relay::hasGlobalId() const
{
    return global_id_.has_value();
}

bool Relay::isOpen() const
{
    return is_open_;
}

void Relay::setGlobalId(GlobalId&& global_id)
{
    global_id_ = std::move(global_id);
}
