#ifndef MQTT_RC_PUBLISHER_HPP
#define MQTT_RC_PUBLISHER_HPP

#include "mqtt/MqttClient.hpp"
#include "rc/Rc.hpp"

class MqttRcPublisher {
    public:
        MqttRcPublisher(MqttClient* mqtt);

        void publishRcSetRc(const Rc& rc) const;

    private:
        MqttClient* mqtt_;
};

#endif
