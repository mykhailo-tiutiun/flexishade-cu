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

    std::string asAscii85()
    {
        char block[5];
        uint32_t v = val;
        for (int t = 4; t >= 0; --t) {
            block[t] = static_cast<char>(v % 85u + 33u);
            v /= 85u;
        }
        return std::string(block, 5);
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
        static constexpr std::size_t serializationSize()
        {
            return sizeof(id_) + sizeof(gpio_num_) + sizeof(is_open_);
        }

    private:
        Relay();

        RelayId id_;
        gpio_num_t gpio_num_;
        bool is_open_;
};

#endif
