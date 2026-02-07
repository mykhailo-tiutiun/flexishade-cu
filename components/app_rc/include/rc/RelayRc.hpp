#ifndef RELAY_RC_HPP
#define RELAY_RC_HPP

#include "relay/Relay.hpp"
#include <cstdint>

class RelayRc
{
    public:
        RelayRc(std::uint64_t addr, RelayId relay_id);
        std::uint64_t getAddr() const;
        const RelayId& getRelayId() const;

        void setRelay(RelayId relay_id);

    private:
        std::uint64_t ieee_addr_;
        RelayId relay_id_;

};

#endif
