#ifndef RELAY_HPP
#define RELAY_HPP

#include "soc/gpio_num.h"
#include <cstddef>
#include <functional>
#include <optional>
#include <string>
#include <variant>


struct RelayId
{
    int val;

    RelayId() : val(0) {}
    explicit RelayId(int v) : val(v) {}

    operator int() const
    {
        return val;
    }

    bool operator<(const RelayId& other) const
    {
        return val < other.val;
    }

    bool operator==(const RelayId& other) const noexcept
    {
        return val == other.val;
    }
};

template <>
struct std::hash<RelayId>
{
    std::size_t operator()(const RelayId& p) const
    {
        return hash<int>{}(p.val);
    }
};

class Relay
{
    private:
        RelayId id_;
        gpio_num_t gpio_num_;
        bool is_open_;

    public:
        Relay(RelayId id, int gpio_num);
        void open();
        void close();
        void toggle();

        const RelayId& getId() const;

        bool isOpen() const;
};

#endif
