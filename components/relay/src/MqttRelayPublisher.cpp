#include "relay/MqttRelayPublisher.hpp"
#include "io/cloud/MqttClient.hpp"
#include <cstdio>
#include <string>

MqttRelayPublisher::MqttRelayPublisher(MqttClient* mqtt)
    : mqtt_(mqtt)
{}

void MqttRelayPublisher::publishStateWithLocalId(const Relay& relay) const
{
    std::string state = relay.isOpen() ? "opened" : "closed";
    std::string data = std::string("{\"control_unit_id\":1,\"relay_local_id\":") + std::to_string(relay.getLocalId().val) + "\",\"state\":" + state + "\"}";
    std::string topic = "cloud/control_unit/local_relay/state";

    mqtt_->publish(topic, data);
}

void MqttRelayPublisher::publishStateWithGlobalId(const Relay& relay) const
{
    std::string state = relay.isOpen() ? "opened" : "closed";
    std::string data = std::string("{\"relay_global_id\":") + std::to_string(relay.getLocalId().val) + "\",\"state\":" + state + "\"}";
    std::string topic = "cloud/relay/state";

    mqtt_->publish(topic, data);
}
