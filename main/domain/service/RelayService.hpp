#ifndef RELAY_SERVICE_HPP
#define RELAY_SERVICE_HPP

#include "../model/Relay.hpp"
#include "../../db/RelayDb.hpp"
#include <string>
#include <vector>

class RelayService {
    public:
        RelayService(RelayDb* relayDb);
        ~RelayService();

        void requestStateById(const RelayId& id);

        void openById(const RelayId& id);
        void closeById(const RelayId& id);
        void toggleById(const RelayId& id);

        std::vector<Relay> getAll();

    private:
        RelayDb* relayDb_;
};

#endif
