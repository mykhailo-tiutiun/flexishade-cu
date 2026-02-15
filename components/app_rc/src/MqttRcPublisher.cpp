#include "rc/MqttRcPublisher.hpp"
#include "mqtt/MqttClient.hpp"
#include "rc/Rc.hpp"
#include "cJSON.h"
#include <string>

MqttRcPublisher::MqttRcPublisher(MqttClient* mqtt)
    : mqtt_(mqtt)
{}

void MqttRcPublisher::publishRcSetRc(const Rc& rc) const
{
    auto json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "cu_id", 1);
    cJSON_AddStringToObject(json, "rc_id", rc.getId().asHexString().c_str());
    cJSON_AddNumberToObject(json, "relay_id", rc.getRelayId());

    std::string topic = "cloud/cu/rc/relay";
    auto payload = std::string(cJSON_PrintUnformatted(json));

    cJSON_Delete(json);

    mqtt_->publish(topic, payload);
}
