#include "relay/RelayDb.hpp"

#include "relay/Relay.hpp"

#include <optional>
#include <variant>
#include <vector>

RelayDb::~RelayDb()
{
    reset();
}

void RelayDb::save(Relay&& relay)
{
    const auto local_id = relay.getLocalId();
    const auto global_id = relay.getGlobalId();

    relays_.insert({local_id, std::move(relay)});

    if (global_id.has_value()) {
        id_mapping_[*global_id] = local_id;
    }
}

void RelayDb::reset()
{
    id_mapping_.clear();
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
    if (const auto *local_id = std::get_if<LocalId>(&id)){
        return getByLocalId(*local_id);
    }

    if (const auto *global_id = std::get_if<GlobalId>(&id)){
        return getByGlobalId(*global_id);
    }

    return std::nullopt;
}

std::optional<Relay> RelayDb::getByGlobalId(const GlobalId& global_id) const
{
    auto it = id_mapping_.find(global_id);

    if (it == id_mapping_.end()) {
        return {};
    }

    return getByLocalId(it->second);
}

std::optional<Relay> RelayDb::getByLocalId(const LocalId& local_id) const
{
    auto it = relays_.find(local_id);

    if (it == relays_.end()) {
        return {};
    }

    return it->second;
}


