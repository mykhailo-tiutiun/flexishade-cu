#ifndef MQTT_CLIENT_HPP
#define MQTT_CLIENT_HPP

#include "io/cloud/MqttSubscribtion.hpp"
#include "config/MqttConfig.hpp"
#include <map>
#include <string>
#include <utility>
#include "mqtt_client.h"

#define MQTT_CONTENT_TYPE_JSON "application/json"

class MqttClient {

    public:
        MqttClient();
        ~MqttClient();

        void configure(MqttConfig config);

        void start();
        void stop();

        void publish(const std::string& topic, const std::string& data) const;
        void subscribe(MqttSubscribtion subscribtion);

        bool isUp() const;

    private:
        MqttConfig config_;
        bool is_up_;
        esp_mqtt_client_handle_t mqtt_client_;

        std::map<std::string, MqttSubscribtion> subscribtions_;

        static void connected_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
        static void data_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
};

#endif
