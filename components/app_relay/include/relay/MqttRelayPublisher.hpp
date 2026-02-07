#ifndef MQTT_RELAY_PUBLISHER_HPP
#define MQTT_RELAY_PUBLISHER_HPP

#include "mqtt/MqttClient.hpp"
#include "relay/Relay.hpp"

class MqttRelayPublisher {
    public:
        MqttRelayPublisher(MqttClient* mqtt);

        void publishRelayState(const Relay& relay) const;

    private:
        MqttClient* mqtt_;
};

#endif
