#ifndef MQTT_RC_CONTROLLER_HPP
#define MQTT_RC_CONTROLLER_HPP

#include "rc/Rc.hpp"
#include "rc/RcService.hpp"
#include "mqtt/MqttClient.hpp"
#include "mqtt/MqttSubscribtion.hpp"
#include <vector>

class MqttRcController {
    public:
        MqttRcController(MqttClient* mqtt, RcService* service);

        void subscribeAll();
    private:
        MqttClient* mqtt_;
        RcService* service_;

        MqttSubscribtion setRcRelaySubsribtion();
        static void setRcRelayHandler(std::string data, void* args);
};

#endif
