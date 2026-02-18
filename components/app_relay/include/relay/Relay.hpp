#ifndef RELAY_HPP
#define RELAY_HPP

#include "soc/gpio_num.h"
#include <cstddef>
#include <cstdint>
#include <expected>
#include <functional>
#include <string>


struct RelayId
{
    std::uint32_t val;

    RelayId() : val(0) {}
    explicit RelayId(std::uint32_t v) : val(v) {}

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
        return hash<std::uint32_t>{}(p.val);
    }
};

class Relay
{
    public:
        Relay(RelayId id, int gpio_num);

        void open();
        void close();
        void toggle();

        const RelayId& getId() const;

        bool isOpen() const;

        std::vector<std::uint8_t> serialize() const;
        static std::expected<Relay, std::string> desirialize(std::vector<std::uint8_t> blob);

    private:
        Relay();

        RelayId id_;
        gpio_num_t gpio_num_;
        bool is_open_;
};

#endif
