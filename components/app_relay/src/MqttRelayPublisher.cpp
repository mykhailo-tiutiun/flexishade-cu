#include "relay/MqttRelayPublisher.hpp"
#include "mqtt/MqttClient.hpp"
#include <cstdio>
#include <string>

MqttRelayPublisher::MqttRelayPublisher(MqttClient* mqtt)
    : mqtt_(mqtt)
{}

void MqttRelayPublisher::publishRelayState(const Relay& relay) const
{
    std::string state = relay.isOpen() ? "opened" : "close";
    std::string data = std::string("{\"control_unit_id\":1,\"relay_id\":") + std::to_string(relay.getId().val) + "\",\"state\":\"" + state + "\"}";
    std::string topic = "cloud/control_unit/relay/state";

    mqtt_->publish(topic, data);
}
