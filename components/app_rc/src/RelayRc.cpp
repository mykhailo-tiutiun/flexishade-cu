
#include "rc/RelayRc.hpp"
#include "relay/Relay.hpp"

RelayRc::RelayRc(std::uint64_t addr, RelayId relay_id)
: ieee_addr_(addr)
, relay_id_(relay_id)
{}

std::uint64_t RelayRc::getAddr() const
{
    return ieee_addr_;
}
const RelayId& RelayRc::getRelayId() const
{
    return relay_id_;
}

void RelayRc::setRelay(RelayId relay_id)
{
    relay_id_ = relay_id;
}

