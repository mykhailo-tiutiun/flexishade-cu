#include "NormalState.hpp"
#include "../StateLed.hpp"
#include "AppContext.hpp"
#include "../net/wifi/WifiSta.hpp"
#include "../mqtt/MqttClient.hpp"

void NormalState::onEnter() {
    
    wifi_sta_ = new WifiSta();
    wifi_sta_->start();

    mqtt_ = new MqttClient();

    context_->getStateLed()->green();
}

void NormalState::onExit() {
    wifi_sta_->stop();

    delete mqtt_;
    mqtt_ = nullptr;

    context_->getStateLed()->amber();

    vTaskDelay(pdMS_TO_TICKS(500)); 
}

void NormalState::toogleConfigMode() {
    AppState* next_state = context_->getAppStateFactory()->config();
    context_->transit_state(next_state);
}

const char* NormalState::getName() const {
    return "Normal";
}