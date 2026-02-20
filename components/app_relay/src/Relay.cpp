#include "relay/Relay.hpp"
#include "driver/gpio.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <expected>
#include <optional>
#include <string>
#include <vector>

Relay::Relay(RelayId id, int gpio_num)
: id_(id)
, gpio_num_((gpio_num_t)gpio_num)
, is_open_(false)
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
    gpio_set_level(gpio_num_, 1);
}

void Relay::close()
{
    is_open_ = false;
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

const RelayId& Relay::getId() const
{
    return id_;
}

bool Relay::isOpen() const
{
    return is_open_;
}

std::vector<std::uint8_t> Relay::serialize() const
{
    auto id_size = sizeof(id_);
    auto gpio_num_size = sizeof(gpio_num_);
    auto is_open_size = sizeof(is_open_);

    std::vector<uint8_t> buf(id_size + gpio_num_size + is_open_size);
    std::size_t off = 0;

    std::memcpy(buf.data() + off, &id_, id_size); off += id_size;
    std::memcpy(buf.data() + off, &gpio_num_, gpio_num_size); off += gpio_num_size;
    std::memcpy(buf.data() + off, &is_open_, is_open_size); off += is_open_size;

    return buf;
}

std::expected<Relay, std::string> Relay::desirialize(std::vector<std::uint8_t> blob)
{

    RelayId id;
    int gpio_num;
    bool is_open;

    auto id_size = sizeof(id);
    auto gpio_num_size = sizeof(gpio_num);
    auto is_open_size = sizeof(is_open);

    if (blob.size() != id_size + gpio_num_size + is_open_size) {
        return std::unexpected("Invalid blob");
    }

    std::size_t off = 0;

    std::memcpy(&id, blob.data() + off, id_size); off += id_size;
    std::memcpy(&gpio_num, blob.data() + off, gpio_num_size); off += gpio_num_size;
    std::memcpy(&is_open, blob.data() + off, is_open_size); off += is_open_size;

    Relay relay(id, gpio_num);

    if (is_open) {
        relay.open();
    }

    return relay;
}

