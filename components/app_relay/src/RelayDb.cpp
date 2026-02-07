#include "relay/RelayDb.hpp"

#include "relay/Relay.hpp"

#include <optional>
#include <vector>

RelayDb::~RelayDb()
{
    reset();
}

void RelayDb::save(Relay&& relay)
{
    auto id = relay.getId();

    relays_.insert_or_assign(id, std::move(relay));
}

void RelayDb::reset()
{
    relays_.clear();
}

std::vector<Relay> RelayDb::getAll() const
{
    std::vector<Relay> vals;
    vals.reserve(relays_.size());

    for (auto& [key, val] : relays_) {
        vals.push_back(val);
    }

    return vals;
}

std::optional<Relay> RelayDb::getById(const RelayId& id) const
{
    auto it = relays_.find(id);

    if (it == relays_.end()) {
        return {};
    }

    return it->second;
}

