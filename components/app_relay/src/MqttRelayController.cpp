#include "relay/MqttRelayController.hpp"
#include "mqtt/MqttClient.hpp"
#include "mqtt/MqttSubscribtion.hpp"
#include <string>
#include "cJSON.h"

MqttRelayController::MqttRelayController(RelayService* service)
: service_(service)
{}

void MqttRelayController::handle(const std::string& method_id, cJSON* payload) const
{
    if (method_id == "set_state") {
        setState(payload);
    }
    if (method_id == "get_state") {
        getState(payload);
    }
}

void MqttRelayController::setState(cJSON* payload) const
{
    cJSON *id_item = cJSON_GetObjectItem(payload, "relay_id");
    cJSON *state_item = cJSON_GetObjectItem(payload, "state");

    if (cJSON_IsNumber(id_item) && cJSON_IsString(state_item)) {
        int id = cJSON_GetNumberValue(id_item);
        std::string state = cJSON_GetStringValue(state_item);

        if (state.compare("open") == 0) {
            service_->openById(RelayId(id));
        }
        if (state.compare("close") == 0) {
            service_->closeById(RelayId(id));
        }
    }
}

void MqttRelayController::getState(cJSON* payload) const
{
    cJSON *id_item = cJSON_GetObjectItem(payload, "relay_id");

    if (cJSON_IsNumber(id_item)) {
        int id = cJSON_GetNumberValue(id_item);

        service_->requestStateById(RelayId(id));
    }

}
