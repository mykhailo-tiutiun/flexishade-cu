#ifndef MQTT_RELAY_CONTROLLER_HPP
#define MQTT_RELAY_CONTROLLER_HPP

#include "relay/Relay.hpp"
#include "relay/RelayService.hpp"
#include "io/cloud/MqttClient.hpp"
#include "io/cloud/MqttSubscribtion.hpp"
#include <vector>

class MqttRelayController {
    public:
        MqttRelayController(MqttClient* mqtt, RelayService* service);

        void subscribeAll();
    private:
        MqttClient* mqtt_;
        RelayService* service_;

        MqttSubscribtion setRelayStateByLocalIdSubsribtion();
        static void setRelayStateByLocalIdHandler(std::string data, void* args);
};

#endif
