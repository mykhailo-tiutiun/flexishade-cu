#include "rc/RcService.hpp"
#include "esp_log.h"
#include "rc/Rc.hpp"
#include <filesystem>

static const char* TAG = "rc service";

RcService::RcService(RcRepository* rc_repository, RelayService* relay_service)
: rc_repository_(rc_repository)
, relay_service_(relay_service)
{}

void RcService::toggleRc(std::uint64_t addr)
{
    ESP_LOGI(TAG, "RC %010x, toggle", addr);

    if (auto rc = rc_repository_->getByAddr(addr)) {
        relay_service_->toggleById(rc->getRelayId());
    } else {
        auto new_rc = newRc(addr);
        relay_service_->toggleById(new_rc.getRelayId());
    }
}

void RcService::setRelay(std::uint64_t addr, RelayId relay_id)
{
    if (auto rc = rc_repository_->getByAddr(addr)) {
        rc->setRelay(relay_id);
    }
}

Rc RcService::newRc(std::uint64_t addr)
{
    ESP_LOGI(TAG, "Create new RC %010x", addr);
    auto rc = Rc(addr, RelayId(1));
    rc_repository_->save(rc);
    return rc;
}
