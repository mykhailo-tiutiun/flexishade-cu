#include "mqtt/MqttClient.hpp"
#include "cJSON.h"
#include "mqtt/MqttController.hpp"
#include "mqtt/MqttSubscribtion.hpp"
#include "esp_event.h"
#include "mqtt/MqttTopicParser.hpp"
#include "mqtt_client.h"
#include "mqtt5_client.h"
#include "esp_log.h"
#include <functional>
#include <optional>
#include <string>

#define MQTT_CLIENT_PUBLISH_QOS 0
#define MQTT_CLIENT_PUBLISH_RETAIN 0

#define ERROR_CHECK(exp, msg) if (exp == ESP_FAIL) { return std::unexpected(msg); }

static const char* TAG = "mqtt_client";

static void log_error_if_nonzero(const char *message, int error_code){
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

static esp_mqtt5_user_property_item_t user_property_arr[] = {
        {"board", "esp32"},
        {"u", "user"},
        {"p", "password"}
    };

#define USE_PROPERTY_ARR_SIZE   sizeof(user_property_arr)/sizeof(esp_mqtt5_user_property_item_t)

static esp_mqtt5_publish_property_config_t publish_property = {
    .payload_format_indicator = 1,
    .message_expiry_interval = 1000,
    .topic_alias = 0,
    .response_topic = "/topic/test/response",
    .correlation_data = "123456",
    .correlation_data_len = 6,
};

static esp_mqtt5_subscribe_property_config_t subscribe_property = {
    .subscribe_id = 25555,
    .no_local_flag = false,
    .retain_as_published_flag = false,
    .retain_handle = 0,
    .is_share_subscribe = true,
    .share_name = "group1",
};

static esp_mqtt5_subscribe_property_config_t subscribe1_property = {
    .subscribe_id = 25555,
    .no_local_flag = true,
    .retain_as_published_flag = false,
    .retain_handle = 0,
};

static esp_mqtt5_unsubscribe_property_config_t unsubscribe_property = {
    .is_share_subscribe = true,
    .share_name = "group1",
};

static esp_mqtt5_disconnect_property_config_t disconnect_property = {
    .session_expiry_interval = 60,
    .disconnect_reason = 0,
};

static void print_user_property(mqtt5_user_property_handle_t user_property) {
    if (user_property) {
        uint8_t count = esp_mqtt5_client_get_user_property_count(user_property);
        if (count) {
            esp_mqtt5_user_property_item_t *item = new esp_mqtt5_user_property_item_t[count];
            if (esp_mqtt5_client_get_user_property(user_property, item, &count) == ESP_OK) {
                for (int i = 0; i < count; i ++) {
                    esp_mqtt5_user_property_item_t *t = &item[i];
                    ESP_LOGI(TAG, "key is %s, value is %s", t->key, t->value);
                    free((char *)t->key);
                    free((char *)t->value);
                }
            }
            delete[] item;
        }
    }
}

static void mqtt5_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32, base, event_id);
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t) event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    ESP_LOGD(TAG, "free heap size is %" PRIu32 ", minimum %" PRIu32, esp_get_free_heap_size(), esp_get_minimum_free_heap_size());
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        esp_mqtt_client_disconnect(client);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        ESP_LOGI(TAG, "MQTT5 return code is %d", event->error_handle->connect_return_code);
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
        }
        break;
    default:
        break;
    }
}

MqttClient::MqttClient()
: start_semaphore_(0)
{}

MqttClient::~MqttClient()
{
    stop();
}

void MqttClient::connected_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");

    auto mqtt_client = (MqttClient*) handler_args;

    esp_mqtt_client_subscribe_single(mqtt_client->mqtt_client_, MQTT_CLIENT_SUBSCRIBE_TOPIC, 0);

    mqtt_client->is_up_ = true;

    mqtt_client->start_semaphore_.release();
}

void MqttClient::disconnected_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");

    auto mqtt_client = (MqttClient*) handler_args;
    mqtt_client->start_semaphore_.release();
}

void MqttClient::data_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGI(TAG, "MQTT_EVENT_DATA");

    auto mqtt_client = (MqttClient*) handler_args;
    auto event = (esp_mqtt_event_handle_t) event_data;

    ESP_LOGI(TAG, "TOPIC=%.*s", event->topic_len, event->topic);
    ESP_LOGI(TAG, "DATA=%.*s", event->data_len, event->data);

    std::string_view mqtt_topic(event->topic, event->topic_len);
    auto payload = cJSON_ParseWithLength(event->data, event->data_len);

    auto parser = MqttTopicParser(MQTT_CLIENT_ID, mqtt_topic);
    if (parser.isValid()) {
        if (auto controller = mqtt_client->getController(parser.getControllerId())) {
            controller.value()->handle(parser.getMethodId(), payload);
        }
    }

    cJSON_Delete(payload);
}


void MqttClient::configure(MqttConfig config)
{
    config_ = std::move(config);
}

std::expected<void, std::string> MqttClient::start()
{
    if (is_up_ == true) {
        return std::unexpected("MqttClient start, unexpected state: up");
    }

    esp_mqtt5_connection_property_config_t connect_property = {
        .session_expiry_interval = 10,
        .maximum_packet_size = 1024,
        .receive_maximum = 65535,
        .topic_alias_maximum = 2,
        .request_resp_info = true,
        .request_problem_info = true,
        .will_delay_interval = 10,
        .message_expiry_interval = 10,
        .payload_format_indicator = true,
        .response_topic = "/test/response",
        .correlation_data = "123456",
        .correlation_data_len = 6,
    };

    esp_mqtt_client_config_t mqtt_cfg = {};
    mqtt_cfg.broker.address.uri = "mqtt://192.168.50.10";
    mqtt_cfg.session.protocol_ver = MQTT_PROTOCOL_V_5;
    mqtt_cfg.network.disable_auto_reconnect = true;
    mqtt_cfg.session.last_will.topic = "/topic/will";
    mqtt_cfg.session.last_will.msg = "i will leave";
    mqtt_cfg.session.last_will.msg_len = 12;
    mqtt_cfg.session.last_will.qos = 1;
    mqtt_cfg.session.last_will.retain = true;


    mqtt_client_ = esp_mqtt_client_init(&mqtt_cfg);

    ERROR_CHECK(esp_mqtt5_client_set_user_property(&connect_property.user_property, user_property_arr, USE_PROPERTY_ARR_SIZE), "MqttClient failed to start");

    ERROR_CHECK(esp_mqtt5_client_set_user_property(&connect_property.will_user_property, user_property_arr, USE_PROPERTY_ARR_SIZE), "MqttClient failed to start");
    ERROR_CHECK(esp_mqtt5_client_set_connect_property(mqtt_client_, &connect_property), "MqttClient failed to start");

    esp_mqtt5_client_delete_user_property(connect_property.user_property);
    esp_mqtt5_client_delete_user_property(connect_property.will_user_property);


    ERROR_CHECK(esp_mqtt_client_register_event(mqtt_client_, (esp_mqtt_event_id_t) ESP_EVENT_ANY_ID, mqtt5_event_handler, NULL), "MqttClient failed to start");
    ERROR_CHECK(esp_mqtt_client_register_event(mqtt_client_, MQTT_EVENT_CONNECTED, connected_event_handler, this), "MqttClient failed to start");
    ERROR_CHECK(esp_mqtt_client_register_event(mqtt_client_, MQTT_EVENT_DISCONNECTED, disconnected_event_handler, this), "MqttClient failed to start");
    ERROR_CHECK(esp_mqtt_client_register_event(mqtt_client_, MQTT_EVENT_DATA, data_event_handler, this), "MqttClient failed to start");

    ERROR_CHECK(esp_mqtt_client_start(mqtt_client_), "MqttClient failed to start");

    start_semaphore_.acquire();

    if (isUp()) {
        return {};
    } else {
        return std::unexpected("MqttClient failed to connect");
    }
}

std::expected<void, std::string> MqttClient::stop()
{
    if (is_up_ == false) {
        return std::unexpected("MqttClient stop, unexpected state: down");
    }

    ERROR_CHECK(esp_mqtt_client_stop(mqtt_client_), "MqttClient failed to stop");
    ERROR_CHECK(esp_mqtt_client_destroy(mqtt_client_), "MqttClient failed to stop");

    is_up_ = false;

    return {};
}

std::expected<void, std::string> MqttClient::publish(const std::string& topic, const std::string& data) const
{
    if (is_up_ == false) {
        return std::unexpected("MqttClient publish, unexpected state: down");
    }

    esp_mqtt_client_publish(mqtt_client_, topic.data(), data.data(), data.length(), MQTT_CLIENT_PUBLISH_QOS, MQTT_CLIENT_PUBLISH_RETAIN);

    return {};
}

void MqttClient::addController(std::shared_ptr<MqttController> controller)
{
    auto id = std::string(controller->getControllerId());

    controllers_.insert({std::move(id), controller});
}

bool MqttClient::isUp() const
{
    return is_up_;
}

std::optional<std::shared_ptr<MqttController>> MqttClient::getController(const std::string& controller_topic)
{
    auto ctrl_it = controllers_.find(controller_topic);
    if (ctrl_it != controllers_.end()) {
        return ctrl_it->second;
    }
    return {};
}
