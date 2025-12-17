#include "app/NormalState.hpp"

#include "app/AppContext.hpp"
#include "io/config/StateLed.hpp"
#include "io/cloud/WifiSta.hpp"
#include "io/cloud/MqttClient.hpp"

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

