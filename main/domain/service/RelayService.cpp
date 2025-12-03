#include "RelayService.hpp"

#include "../model/Relay.hpp"
#include "../../db/RelayDb.hpp"
#include <vector>

RelayService::RelayService(RelayDb* relayDb, MqttRelayPublisher* mqtt_publisher)
    : relayDb_(relayDb)
    , mqtt_publisher_(mqtt_publisher)
{}

RelayService::~RelayService() {}

std::vector<Relay> RelayService::getAll() const
{
    return relayDb_->getAll();
}

void RelayService::requestStateById(const RelayId& id) const
{
    auto orelay = relayDb_->getById(id);
    if (orelay.has_value()) {
        Relay relay = orelay.value();
        publishState(relay);
    }
}

void RelayService::openById(const RelayId& id)
{
    auto orelay = relayDb_->getById(id);
    if (orelay.has_value()) {
        Relay relay = orelay.value();
        relay.open();
        publishState(relay);
        relayDb_->save(std::move(relay));
    }
}

void RelayService::closeById(const RelayId& id)
{
    auto orelay = relayDb_->getById(id);
    if (orelay.has_value()) {
        Relay relay = orelay.value();
        relay.close();
        publishState(relay);
        relayDb_->save(std::move(relay));
    }
}

void RelayService::toggleById(const RelayId& id)
{
    auto orelay = relayDb_->getById(id);
    if (orelay.has_value()) {
        Relay relay = orelay.value();
        relay.toggle();
        publishState(relay);
        relayDb_->save(std::move(relay));
    }
}


void RelayService::publishState(const Relay& relay) const
{
    if (relay.hasGlobalId()){
        mqtt_publisher_->publishStateWithGlobalId(relay);
    } else {
        mqtt_publisher_->publishStateWithLocalId(relay);
    }
}
