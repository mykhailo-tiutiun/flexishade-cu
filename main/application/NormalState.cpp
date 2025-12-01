#include "NormalState.hpp"
#include "../StateLed.hpp"
#include "AppContext.hpp"
#include "../net/wifi/WifiSta.hpp"
#include "../mqtt/MqttClient.hpp"

void NormalState::onEnter() {

    wifi_sta_ = new WifiSta();
    wifi_sta_->start();

    if (auto mqtt = context_->tryGetComponent<MqttClient>()){
        mqtt.value()->start();
    }

    context_->tryGetComponent<StateLed>().value()->green();
}

void NormalState::onExit() {
    context_->tryGetComponent<StateLed>().value()->amber();

    if (auto mqtt = context_->tryGetComponent<MqttClient>()){
        mqtt.value()->stop();
    }

    wifi_sta_->stop();
}

void NormalState::toogleConfigMode() {
    context_->transit_state(CONFIG);
}

