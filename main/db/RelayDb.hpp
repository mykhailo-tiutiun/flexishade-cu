#ifndef RELAY_DB_HPP
#define RELAY_DB_HPP

#include "../domain/model/Relay.hpp"
#include <map>
#include <vector>

class RelayDb {
    public:
        ~RelayDb();

        std::optional<Relay> getById(const RelayId& id) const;
        std::vector<Relay> getAll() const;

        void save(Relay&& relay);

        void reset();

    private:
        std::map<LocalId, Relay> relays_;
        std::map<GlobalId, LocalId> id_mapping_;

        std::optional<Relay> getByLocalId(const LocalId& id) const;
        std::optional<Relay> getByGlobalId(const GlobalId& id) const;
};

#endif
