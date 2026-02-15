#include "rc/MqttRcController.hpp"
#include "rc/Rc.hpp"
#include "rc/RcService.hpp"
#include <cstdint>
#include <string>
#include "cJSON.h"
#include "relay/Relay.hpp"

MqttRcController::MqttRcController(RcService* service)
: service_(service)
{}

void MqttRcController::handle(const std::string& method_id, cJSON* payload) const
{
    if (method_id == "set_relay") {
        setRcRelay(payload);
    }
}

void MqttRcController::setRcRelay(cJSON* payload) const
{
    cJSON *rc_id_json = cJSON_GetObjectItem(payload, "rc_id");
    cJSON *relay_id_json = cJSON_GetObjectItem(payload, "relay_id");

    if (cJSON_IsString(rc_id_json) && cJSON_IsNumber(relay_id_json)) {
        std::uint64_t rc_id = std::stoull(cJSON_GetStringValue(rc_id_json), nullptr, 16);
        int relay_id = cJSON_GetNumberValue(relay_id_json);

        service_->setRelay(RcId(rc_id), RelayId(relay_id));
    }
}
