#ifndef MQTT_RELAY_PUBLISHER_HPP
#define MQTT_RELAY_PUBLISHER_HPP

#include "MqttClient.hpp"
#include "../domain/model/Relay.hpp"

class MqttRelayPublisher {
    public:
        MqttRelayPublisher(MqttClient* mqtt);

        void publishStateWithLocalId(const Relay& relay) const;
        void publishStateWithGlobalId(const Relay& relay) const;

    private:
        MqttClient* mqtt_;
};

#endif
