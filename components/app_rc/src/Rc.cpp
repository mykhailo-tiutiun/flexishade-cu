
#include "rc/Rc.hpp"
#include "relay/Relay.hpp"

Rc::Rc(RcId id, RelayId relay_id)
: id_(id)
, relay_id_(relay_id)
{}

RcId Rc::getId() const
{
    return id_;
}

RelayId Rc::getRelayId() const
{
    return relay_id_;
}

void Rc::setRelay(RelayId relay_id)
{
    relay_id_ = relay_id;
}

