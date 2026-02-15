#ifndef MQTT_CONTROLLER_HPP
#define MQTT_CONTROLLER_HPP

#include "cJSON.h"
#include <string>

class MqttController
{
    public:
        virtual std::string_view getControllerId() const = 0;
        virtual void handle(const std::string& method_id, cJSON* payload) const = 0;
};

#endif
