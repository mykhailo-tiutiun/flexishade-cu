
#include "rc/Rc.hpp"
#include "esp_log.h"
#include "relay/Relay.hpp"
#include <cstring>

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

std::vector<std::uint8_t> Rc::serialize() const
{
    auto id_size = sizeof(id_);
    auto relay_id = sizeof(relay_id_);

    std::vector<std::uint8_t> buf(id_size + relay_id);
    std::size_t off = 0;

    std::memcpy(buf.data() + off, &id_, id_size); off += id_size;
    std::memcpy(buf.data() + off, &relay_id_, relay_id); off += relay_id;

    return buf;
}

std::expected<Rc, std::string> Rc::desirialize(std::vector<std::uint8_t> blob)
{

    RcId id;
    RelayId relay_id;

    auto id_size = sizeof(id);
    auto relay_id_size = sizeof(relay_id);

    if (blob.size() != id_size + relay_id_size) {
        return std::unexpected("Invalid blob");
    }

    std::size_t off = 0;

    std::memcpy(&id, blob.data() + off, id_size); off += id_size;
    std::memcpy(&relay_id, blob.data() + off, relay_id_size); off += relay_id_size;

    Rc rc(id, relay_id);

    return rc;
}
