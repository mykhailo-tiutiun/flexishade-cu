#include "rc/RcService.hpp"
#include "esp_log.h"
#include "rc/Rc.hpp"
#include "rc/RcRepository.hpp"
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

    auto rc_res = rc_repository_->getById(id);

    if (!rc_res) {
        auto err = rc_res.error();
        if (err.kind() == RcRepositoryError::NotFound) {
            ESP_LOGW(TAG, "Toggle RC, RC not found, creating new ...");
            auto new_rc = newRc(id);
            relay_service_->toggleById(new_rc.getRelayId());
            return;
        } else {
            ESP_LOGE(TAG, "Toggle RC, unexpected error: %s", err.msg().c_str());
            return;
        }
    }

    relay_service_->toggleById((*rc_res).getRelayId());
}

void RcService::setRelay(RcId id, RelayId relay_id)
{
    ESP_LOGI(TAG, "RC 0x%016" PRIX64 ", set Relay %d", id, relay_id);

    auto rc_res = rc_repository_->getById(id);

    if (!rc_res) {
        auto err = rc_res.error();
        if (err.kind() == RcRepositoryError::NotFound) {
            ESP_LOGW(TAG, "Set RC's Relay, RC not found: %s", err.msg().c_str());
            return;
        } else {
            ESP_LOGE(TAG, "Set RC's Relay, unexpected error: %s", err.msg().c_str());
            return;
        }
    }

    (*rc_res).setRelay(relay_id);
    rc_repository_->save(*rc_res);

    rc_publisher_->publishRcSetRc(*rc_res);
}

Rc RcService::newRc(RcId id)
{
    ESP_LOGI(TAG, "Create new RC 0x%016" PRIX64 " also %ul", id, id);
    auto rc = Rc(id, RelayId(1));
    auto res = rc_repository_->save(rc);

    if (!res) {
        auto err = res.error();
        ESP_LOGE(TAG, "Save RC, unexpected error: %s", err.msg().c_str());
    }

    return rc;
}
