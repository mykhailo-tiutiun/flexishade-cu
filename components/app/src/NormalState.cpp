#include "app/NormalState.hpp"

#include "app/AppContext.hpp"
#include "config/ConfigStorage.hpp"
#include "io/cloud/MqttClient.hpp"
#include "io/config/StateLed.hpp"
#include "io/cloud/WifiSta.hpp"

void NormalState::onEnter()
{
    auto configs = context_->tryGetComponent<ConfigStorage>().value();

    wifi_sta_ = new WifiSta();
    wifi_sta_->configure(configs->getConfig<WifiConfig>("wifi").value());
    wifi_sta_->start();

    if (auto mqtt = context_->tryGetComponent<MqttClient>()){
        mqtt.value()->start();
    }

    context_->tryGetComponent<StateLed>().value()->green();
}

void NormalState::onExit()
{
    context_->tryGetComponent<StateLed>().value()->amber();

    if (auto mqtt = context_->tryGetComponent<MqttClient>()){
        mqtt.value()->stop();
    }

    if (wifi_sta_->isUp()){
        wifi_sta_->stop();
    }
}

void NormalState::toogleConfigMode() {
    context_->transit_state(CONFIG);
}

