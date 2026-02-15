#ifndef MQTT_CLIENT_HPP
#define MQTT_CLIENT_HPP

#include "mqtt/MqttController.hpp"
#include "config/MqttConfig.hpp"
#include <expected>
#include <functional>
#include <memory>
#include <optional>
#include <semaphore>
#include <string>
#include <unordered_map>
#include "mqtt_client.h"

#define MQTT_CLIENT_ID "dev/cu/1"
#define MQTT_CLIENT_SUBSCRIBE_TOPIC MQTT_CLIENT_ID"/#"

#define MQTT_CONTENT_TYPE_JSON "application/json"

class MqttClient {

    public:
        MqttClient();
        ~MqttClient();

        void configure(MqttConfig config);

        std::expected<void, std::string> start();
        std::expected<void, std::string> stop();

        std::expected<void, std::string> publish(const std::string& topic, const std::string& data) const;

        void addController(std::shared_ptr<MqttController> controller);

        bool isUp() const;

    private:
        MqttConfig config_;
        bool is_up_;
        esp_mqtt_client_handle_t mqtt_client_;
        std::binary_semaphore start_semaphore_;

        std::unordered_map<std::string, std::shared_ptr<MqttController>> controllers_;

        static void connected_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
        static void disconnected_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
        static void data_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

        std::optional<std::shared_ptr<MqttController>> getController(const std::string& controller_id);
};

#endif
