#ifndef RELAY_DB_HPP
#define RELAY_DB_HPP

#include "relay/Relay.hpp"
#include <map>
#include <unordered_map>
#include <vector>

class RelayDb {
    public:
        ~RelayDb();

        std::optional<Relay> getById(const RelayId& id) const;
        std::vector<Relay> getAll() const;

        void save(Relay&& relay);

        void reset();

    private:
        std::unordered_map<RelayId, Relay> relays_;
};

#endif
