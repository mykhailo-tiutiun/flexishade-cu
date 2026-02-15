#ifndef MQTT_RC_CONTROLLER_HPP
#define MQTT_RC_CONTROLLER_HPP

#include "cJSON.h"
#include "mqtt/MqttController.hpp"
#include "rc/RcService.hpp"

class MqttRcController : public MqttController {
    public:
        MqttRcController(RcService* service);

        std::string_view getControllerId() const override
        {
            return "rc";
        };

        void handle(const std::string& method_id, cJSON* payload) const override;
    private:
        RcService* service_;

        void setRcRelay(cJSON* payload) const;
};

#endif
