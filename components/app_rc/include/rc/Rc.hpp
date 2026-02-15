#ifndef RC_HPP
#define RC_HPP

#include "relay/Relay.hpp"
#include <cstdint>
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
        return std::format("0x{:016x}", val);
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

    private:
        RcId id_;
        RelayId relay_id_;
};

#endif
