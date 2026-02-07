#ifndef RELAY_SERVICE_HPP
#define RELAY_SERVICE_HPP

#include "relay/Relay.hpp"
#include "relay/RelayDb.hpp"
#include "relay/MqttRelayPublisher.hpp"

#include <string>
#include <vector>

class RelayService {
    public:
        RelayService(RelayDb* relayDb, MqttRelayPublisher* mqtt_publisher);
        ~RelayService();

        std::vector<Relay> getAll() const;

        void requestStateById(const RelayId& id) const;

        void openById(const RelayId& id);
        void closeById(const RelayId& id);
        void toggleById(const RelayId& id);

    private:
        RelayDb* relayDb_;
        MqttRelayPublisher* mqtt_publisher_;

        void publishState(const Relay& relay) const;
};

#endif
