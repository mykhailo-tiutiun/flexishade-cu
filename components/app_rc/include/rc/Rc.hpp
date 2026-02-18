#ifndef RC_HPP
#define RC_HPP

#include "relay/Relay.hpp"
#include <cstdint>
#include <cstring>
#include <format>

struct RcId
{
    std::uint64_t val;

    RcId()
    : val(0)
    {}

    explicit RcId(std::uint64_t v)
    : val(v)
    {}

    operator std::uint64_t() const
    {
        return val;
    }

    std::string asHexString() const
    {
        return std::format("{:016x}", val);
    }

    std::string asAscii85()
    {
        std::array<unsigned char, 8> bytes{
            static_cast<unsigned char>((val >> 56) & 0xFF),
            static_cast<unsigned char>((val >> 48) & 0xFF),
            static_cast<unsigned char>((val >> 40) & 0xFF),
            static_cast<unsigned char>((val >> 32) & 0xFF),
            static_cast<unsigned char>((val >> 24) & 0xFF),
            static_cast<unsigned char>((val >> 16) & 0xFF),
            static_cast<unsigned char>((val >> 8)  & 0xFF),
            static_cast<unsigned char>( val        & 0xFF),
        };

        std::string out;
        out.reserve(12);

        for (std::size_t i = 0; i < bytes.size(); i += 4)
        {
            uint32_t v = (uint32_t(bytes[i]) << 24) |
                         (uint32_t(bytes[i+1]) << 16) |
                         (uint32_t(bytes[i+2]) << 8) |
                         uint32_t(bytes[i+3]);

            char block[5];
            for (int t = 4; t >= 0; --t)
            {
                block[t] = static_cast<char>(v % 85 + 33);
                v /= 85;
            }
            out.append(block, 5);
        }
        return out;
    }

    bool operator<(const RcId& other) const
    {
        return val < other.val;
    }

    bool operator==(const RcId& other) const
    {
        return val == other.val;
    }
};

template <>
struct std::hash<RcId>
{
    std::size_t operator()(const RcId& p) const
    {
        return hash<int>{}(p.val);
    }
};

class Rc
{
    public:
        Rc(RcId id, RelayId relay_id);

        RcId getId() const;
        RelayId getRelayId() const;

        void setRelay(RelayId relay_id);

        std::vector<std::uint8_t> serialize() const;
        static std::expected<Rc, std::string> desirialize(std::vector<std::uint8_t> blob);
        static constexpr std::size_t serializationSize()
        {
            return sizeof(id_) + sizeof(relay_id_);
        }


    private:
        RcId id_;
        RelayId relay_id_;
};

#endif
