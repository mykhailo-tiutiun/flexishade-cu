#ifndef MQTT_RELAY_CONTROLLER_HPP
#define MQTT_RELAY_CONTROLLER_HPP

#include "cJSON.h"
#include "relay/RelayService.hpp"
#include "mqtt/MqttClient.hpp"
#include "mqtt/MqttSubscribtion.hpp"

class MqttRelayController : public MqttController {
    public:
        MqttRelayController(RelayService* service);

        std::string_view getControllerId() const override
        {
            return "relay";
        };

        void handle(const std::string& method_id, cJSON* payload) const override;

    private:
        MqttClient* mqtt_;
        RelayService* service_;

        void setState(cJSON* payload) const;
        void getState(cJSON* payload) const;
};

#endif
