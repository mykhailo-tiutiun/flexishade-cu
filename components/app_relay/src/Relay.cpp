#include "relay/Relay.hpp"
#include "driver/gpio.h"
#include "esp_log.h"
#include <cstdio>
#include <optional>
#include <string>

static const char* TAG = "relay";

Relay::Relay(RelayId id, int gpio_num)
    : id_(id),
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
    ESP_LOGI(TAG, "Relay %d, open", id_);
    is_open_ = true;
    gpio_set_level(gpio_num_, 1);
}

void Relay::close()
{
    ESP_LOGI(TAG, "Relay %d, close", id_);
    is_open_ = false;
    gpio_set_level(gpio_num_, 0);
}

void Relay::toggle()
{
    ESP_LOGI(TAG, "Relay %d, toggle", id_);
    if (is_open_) {
        close();
    } else {
        open();
    }
}

const RelayId& Relay::getId() const
{
    return id_;
}

bool Relay::isOpen() const
{
    return is_open_;
}

