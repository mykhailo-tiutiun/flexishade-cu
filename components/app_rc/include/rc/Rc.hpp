#ifndef RC_HPP
#define RC_HPP

#include "relay/Relay.hpp"
#include <cstdint>

class Rc
{
    public:
        Rc(std::uint64_t addr, RelayId relay_id);
        std::uint64_t getAddr() const;
        const RelayId& getRelayId() const;

        void setRelay(RelayId relay_id);

    private:
        std::uint64_t ieee_addr_;
        RelayId relay_id_;

};

#endif
