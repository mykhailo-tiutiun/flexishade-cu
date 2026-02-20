#include "relay/RelayService.hpp"

#include "esp_log.h"
#include "relay/Relay.hpp"
#include "relay/MqttRelayPublisher.hpp"
#include "relay/RelayRepository.hpp"

#define ASING_CHECK(result, exp) \
    auto res = exp; \
    if (!res) { \
        auto err = res.error(); \
        if (err.kind() == RelayRepositoryError::NotFound) { \
            ESP_LOGW(TAG, "Relay not found"); \
            return; \
        } else { \
            ESP_LOGE(TAG, "Unexpected error: %s", err.msg().c_str()); \
            return; \
        } \
    } \
    auto result = *res;

static const char* TAG = "relay_service";

RelayService::RelayService(RelayRepository* relay_repository, MqttRelayPublisher* mqtt_publisher)
    : relay_repository_(relay_repository)
    , mqtt_publisher_(mqtt_publisher)
{}

void RelayService::requestStateById(const RelayId& id) const
{
    ESP_LOGI(TAG, "Relay %d request state", id);

    ASING_CHECK(relay, relay_repository_->getById(id));
    mqtt_publisher_->publishRelayState(relay);
}

void RelayService::openById(const RelayId& id)
{
    ESP_LOGI(TAG, "Relay %d open", id);

    ASING_CHECK(relay, relay_repository_->getById(id));
    relay.open();
    relay_repository_->save(relay);
    mqtt_publisher_->publishRelayState(relay);
}

void RelayService::closeById(const RelayId& id)
{
    ESP_LOGI(TAG, "Relay %d close", id);

    ASING_CHECK(relay, relay_repository_->getById(id));
    relay.close();
    relay_repository_->save(relay);
    mqtt_publisher_->publishRelayState(relay);
}

void RelayService::toggleById(const RelayId& id)
{
    ESP_LOGI(TAG, "Relay %d toggle", id);

    ASING_CHECK(relay, relay_repository_->getById(id));
    relay.toggle();
    relay_repository_->save(relay);
    mqtt_publisher_->publishRelayState(relay);
}
