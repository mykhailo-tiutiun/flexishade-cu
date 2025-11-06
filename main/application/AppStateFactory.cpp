#include "AppStateFactory.hpp"
#include "AppState.hpp"
#include "ConfigState.hpp"
#include "InitState.hpp"
#include "NormalState.hpp"
#include "ZigbeeInitState.hpp"


AppState* AppStateFactory::init() const {
    return new InitState();
}

AppState* AppStateFactory::zigbeeInit() const {
    return new ZigbeeInitState();
}

AppState* AppStateFactory::normal() const {
    return new NormalState();
}

AppState* AppStateFactory::config() const {
    return new ConfigState();
}