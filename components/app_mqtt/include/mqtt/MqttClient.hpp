#ifndef MQTT_CLIENT_HPP
#define MQTT_CLIENT_HPP

#include "mqtt/MqttSubscribtion.hpp"
#include "config/MqttConfig.hpp"
#include <condition_variable>
#include <expected>
#include <functional>
#include <map>
#include <optional>
#include <semaphore>
#include <string>
#include <utility>
#include "mqtt_client.h"

#define MQTT_CONTENT_TYPE_JSON "application/json"

class MqttClient {

    public:
        MqttClient();
        ~MqttClient();

        void configure(MqttConfig config);

        std::expected<void, std::string> start();
        std::expected<void, std::string> stop();

        std::expected<void, std::string> publish(const std::string& topic, const std::string& data) const;

        void subscribe(MqttSubscribtion subscribtion);

        bool isUp() const;

    private:
        MqttConfig config_;
        bool is_up_;
        esp_mqtt_client_handle_t mqtt_client_;
        std::binary_semaphore start_semaphore_;

        std::map<std::string, MqttSubscribtion> subscribtions_;

        static void connected_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
        static void disconnected_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
        static void data_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

        std::optional<std::reference_wrapper<const MqttSubscribtion>> getSubscribtion(const std::string& topic);
};

#endif
