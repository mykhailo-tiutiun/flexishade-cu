#include "rc/RcService.hpp"
#include "esp_log.h"
#include "rc/Rc.hpp"
#include "relay/Relay.hpp"
#include <cinttypes>

static const char* TAG = "rc_service";

RcService::RcService(RcRepository* rc_repository, MqttRcPublisher* rc_publisher, RelayService* relay_service)
: rc_repository_(rc_repository)
, rc_publisher_(rc_publisher)
, relay_service_(relay_service)
{}

void RcService::toggleRc(RcId id)
{
    ESP_LOGI(TAG, "RC 0x%016" PRIX64 ", toggle", id);

    if (auto rc = rc_repository_->getById(id)) {
        relay_service_->toggleById((*rc).getRelayId());
    } else {
        auto new_rc = newRc(id);
        relay_service_->toggleById(new_rc.getRelayId());
    }
}

void RcService::setRelay(RcId id, RelayId relay_id)
{
    ESP_LOGI(TAG, "Entered");
    ESP_LOGI(TAG, "Found RC 0x%016" PRIX64 ", set Relay %d", id, relay_id);
    if (auto rc = rc_repository_->getById(id)) {
        (*rc).setRelay(relay_id);
        ESP_LOGI(TAG, "Found RC 0x%016" PRIX64 ", set Relay %d", id, relay_id);
        rc_publisher_->publishRcSetRc(*rc);
        rc_repository_->save(*rc);
    }
}

Rc RcService::newRc(RcId id)
{
    ESP_LOGI(TAG, "Create new RC 0x%016" PRIX64 " or %ul", id, id);
    auto rc = Rc(id, RelayId(1));
    rc_repository_->save(rc);
    return rc;
}
