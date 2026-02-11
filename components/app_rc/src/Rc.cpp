
#include "rc/Rc.hpp"
#include "relay/Relay.hpp"

Rc::Rc(std::uint64_t addr, RelayId relay_id)
: ieee_addr_(addr)
, relay_id_(relay_id)
{}

std::uint64_t Rc::getAddr() const
{
    return ieee_addr_;
}
const RelayId& Rc::getRelayId() const
{
    return relay_id_;
}

void Rc::setRelay(RelayId relay_id)
{
    relay_id_ = relay_id;
}

