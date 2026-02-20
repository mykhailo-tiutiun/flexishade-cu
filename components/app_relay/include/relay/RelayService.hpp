#ifndef RELAY_SERVICE_HPP
#define RELAY_SERVICE_HPP

#include "relay/Relay.hpp"
#include "relay/MqttRelayPublisher.hpp"
#include "relay/RelayRepository.hpp"

#include <vector>

class RelayService {
    public:
        RelayService(RelayRepository* relayDb, MqttRelayPublisher* mqtt_publisher);

        void requestStateById(const RelayId& id) const;

        void openById(const RelayId& id);
        void closeById(const RelayId& id);
        void toggleById(const RelayId& id);

    private:
        RelayRepository* relay_repository_;
        MqttRelayPublisher* mqtt_publisher_;
};

#endif
