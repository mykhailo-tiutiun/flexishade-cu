#include "rc/MqttRcController.hpp"
#include "mqtt/MqttClient.hpp"
#include "mqtt/MqttSubscribtion.hpp"
#include "rc/RcService.hpp"
#include <cstdint>
#include <string>
#include "cJSON.h"

MqttRcController::MqttRcController(MqttClient* mqtt, RcService* service)
    : mqtt_(mqtt)
    , service_(service)
{}

void MqttRcController::subscribeAll()
{
    mqtt_->subscribe(setRcRelaySubsribtion());
}

// dev/cu/{cu_id}/rc/set_relay
MqttSubscribtion MqttRcController::setRcRelaySubsribtion()
{
    std::string topic = std::string("dev/cu/") + "1" + "/rc/set_relay";
    return MqttSubscribtion(topic, MQTT_CONTENT_TYPE_JSON, setRcRelayHandler, service_);
}

void MqttRcController::setRcRelayHandler(std::string data, void* args)
{
    RcService* service = (RcService*) args;

    cJSON *root = cJSON_Parse(data.data());
    if (root) {
        cJSON *rc_addr_json = cJSON_GetObjectItem(root, "rc_addr");
        cJSON *relay_id_json = cJSON_GetObjectItem(root, "relay_id");

        if (cJSON_IsNumber(rc_addr_json) && cJSON_IsNumber(relay_id_json)) {
            std::uint64_t rc_addr = cJSON_GetNumberValue(rc_addr_json);
            auto relay_id = RelayId(cJSON_GetNumberValue(relay_id_json));

            service->setRelay(rc_addr, relay_id);
        }
    }

    cJSON_Delete(root);
}

