#ifndef MQTT_RELAY_CONTROLLER_HPP
#define MQTT_RELAY_CONTROLLER_HPP

#include "../domain/model/Relay.hpp"
#include "../domain/service/RelayService.hpp"
#include "MqttClient.hpp"
#include "MqttSubscribtion.hpp"
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
