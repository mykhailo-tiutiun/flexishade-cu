#include "MqttRelayController.hpp"
#include "MqttClient.hpp"
#include "MqttSubscribtion.hpp"
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

MqttSubscribtion MqttRelayController::setRelayStateByLocalIdSubsribtion()
{
    std::string topic = std::string("dev/control_unit/") + "1" + "/local_relay/set_state";
    return MqttSubscribtion(topic, MQTT_CONTENT_TYPE_JSON, setRelayStateByLocalIdHandler, service_);
}

void MqttRelayController::setRelayStateByLocalIdHandler(std::string data, void* args)
{
    RelayService* service = (RelayService*) args;

    cJSON *root = cJSON_Parse(data.data());
    if (root) {
        cJSON *id_item = cJSON_GetObjectItem(root, "local_id");
        cJSON *state_item = cJSON_GetObjectItem(root, "state");

        if (cJSON_IsNumber(id_item) && cJSON_IsString(state_item)) {
            int id = cJSON_GetNumberValue(id_item);
            std::string state = cJSON_GetStringValue(state_item);

            if (state.compare("open") == 0) {
                service->openById(LocalId(id));
            }
            if (state.compare("close") == 0) {
                service->closeById(LocalId(id));
            }
        }
    }

    cJSON_Delete(root);
}

