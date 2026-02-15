#include "relay/MqttRelayPublisher.hpp"
#include "mqtt/MqttClient.hpp"
#include "cJSON.h"
#include <string>

MqttRelayPublisher::MqttRelayPublisher(MqttClient* mqtt)
    : mqtt_(mqtt)
{}

void MqttRelayPublisher::publishRelayState(const Relay& relay) const
{
    auto json = cJSON_CreateObject();

    std::string state = relay.isOpen() ? "opened" : "close";

    cJSON_AddNumberToObject(json, "cu_id", 1);
    cJSON_AddNumberToObject(json, "relay_id", relay.getId());
    cJSON_AddStringToObject(json, "state", state.c_str());

    std::string topic = "cloud/cu/relay/state";
    auto payload = std::string(cJSON_PrintUnformatted(json));

    cJSON_Delete(json);

    mqtt_->publish(topic, payload);
}
