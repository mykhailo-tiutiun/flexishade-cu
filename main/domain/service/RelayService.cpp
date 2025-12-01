#include "RelayService.hpp"

#include "../model/Relay.hpp"
#include "../../db/RelayDb.hpp"
#include <vector>

RelayService::RelayService(RelayDb* relayDb)
    : relayDb_(relayDb)
{}

RelayService::~RelayService() {}

void RelayService::requestStateById(const RelayId& id)
{
    auto orelay = relayDb_->getById(id);
    if (orelay.has_value()) {
        Relay relay = orelay.value();
        relay.isOpen();
    }
}

void RelayService::openById(const RelayId& id)
{
    auto orelay = relayDb_->getById(id);
    if (orelay.has_value()) {
        Relay relay = orelay.value();
        relay.open();
        relayDb_->save(std::move(relay));
    }
}
void RelayService::closeById(const RelayId& id)
{
    auto orelay = relayDb_->getById(id);
    if (orelay.has_value()) {
        Relay relay = orelay.value();
        relay.close();
        relayDb_->save(std::move(relay));
    }
}
void RelayService::toggleById(const RelayId& id)
{
    auto orelay = relayDb_->getById(id);
    if (orelay.has_value()) {
        Relay relay = orelay.value();
        relay.toggle();
        relayDb_->save(std::move(relay));
    }
}

std::vector<Relay> RelayService::getAll()
{
    return relayDb_->getAll();
}
