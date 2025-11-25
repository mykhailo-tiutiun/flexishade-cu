#ifndef MQTT_CLIENT_HPP
#define MQTT_CLIENT_HPP

#define MQTT_CLIENT_BROKER_URL "mqtt://192.168.10.2"


class MqttClient {
    private:
        bool is_up_;
    public:
        MqttClient();
        ~MqttClient();
        bool isUp() const;
};

#endif