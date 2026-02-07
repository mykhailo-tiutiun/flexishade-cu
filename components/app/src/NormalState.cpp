#include "app/NormalState.hpp"

#include "app/AppContext.hpp"
#include "app/AppState.hpp"
#include "config/ConfigService.hpp"
#include "mqtt/MqttClient.hpp"
#include "io/config/StateLed.hpp"
#include "wifi/WifiSta.hpp"

static const char* TAG = "normal_state";

#define ERROR_CHECK(exp) \
    { \
        auto result = exp; \
        if (!result) { \
            ESP_LOGE(TAG, "Error: %s", result.error().c_str()); \
            context_->transit_state(LOCAL_ONLY); \
            return; \
        } \
    }

void NormalState::onEnter()
{
    auto configs = context_->tryGetComponent<ConfigService>().value();

    wifi_sta_ = context_->tryGetComponent<WifiSta>().value();
    wifi_sta_->configure(configs->getConfig<WifiConfig>("wifi").value());

    ERROR_CHECK(wifi_sta_->start());

    ERROR_CHECK(context_->tryGetComponent<MqttClient>().value()->start());

    context_->tryGetComponent<StateLed>().value()->green();
}

void NormalState::onExit()
{
    context_->tryGetComponent<StateLed>().value()->amber();

    if(context_->tryGetComponent<MqttClient>().value()->isUp()){
        ERROR_CHECK(context_->tryGetComponent<MqttClient>().value()->stop());
    }

    if(context_->tryGetComponent<WifiSta>().value()->isUp()){
        ERROR_CHECK(context_->tryGetComponent<WifiSta>().value()->stop());
    }
}

void NormalState::toogleConfigMode() {
    context_->transit_state(CONFIG);
}

