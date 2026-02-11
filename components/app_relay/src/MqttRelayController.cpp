#include "relay/MqttRelayController.hpp"
#include "mqtt/MqttClient.hpp"
#include "mqtt/MqttSubscribtion.hpp"
#include <string>
#include "cJSON.h"

MqttRelayController::MqttRelayController(MqttClient* mqtt, RelayService* service)
    : mqtt_(mqtt)
    , service_(service)
{}

void MqttRelayController::subscribeAll()
{
    mqtt_->subscribe(setRelayStateByLocalIdSubsribtion());
}

// dev/cu/{cu_id}/relay/set_state
MqttSubscribtion MqttRelayController::setRelayStateByLocalIdSubsribtion()
{
    std::string topic = std::string("dev/cu/") + "1" + "/relay/set_state";
    return MqttSubscribtion(topic, MQTT_CONTENT_TYPE_JSON, setRelayStateByLocalIdHandler, service_);
}

void MqttRelayController::setRelayStateByLocalIdHandler(std::string data, void* args)
{
    RelayService* service = (RelayService*) args;

    cJSON *root = cJSON_Parse(data.data());
    if (root) {
        cJSON *id_item = cJSON_GetObjectItem(root, "relay_id");
        cJSON *state_item = cJSON_GetObjectItem(root, "state");

        if (cJSON_IsNumber(id_item) && cJSON_IsString(state_item)) {
            int id = cJSON_GetNumberValue(id_item);
            std::string state = cJSON_GetStringValue(state_item);

            if (state.compare("open") == 0) {
                service->openById(RelayId(id));
            }
            if (state.compare("close") == 0) {
                service->closeById(RelayId(id));
            }
        }
    }

    cJSON_Delete(root);
}

