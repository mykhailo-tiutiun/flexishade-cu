#include "app/ConfigState.hpp"

#include "app/AppContext.hpp"
#include "config/ConfigService.hpp"
#include "esp_log.h"
#include "io/config/StateLed.hpp"
#include "wifi/WifiAp.hpp"
#include "http/HttpServer.hpp"
#include "dns/DnsServer.hpp"

static const char* TAG = "config state";

#define ERROR_CHECK(exp) \
    { \
        auto result = exp; \
        if (!result) { \
            ESP_LOGE(TAG, "Error: %s", result.error().c_str()); \
            context_->transit_state(CRITICAL); \
            return; \
        } \
    }

ConfigState::ConfigState() {}

void ConfigState::onEnter() {

    ERROR_CHECK(context_->tryGetComponent<WifiAp>().value()->start());
    ERROR_CHECK(context_->tryGetComponent<HttpServer>().value()->start());
    ERROR_CHECK(context_->tryGetComponent<DnsServer>().value()->start());

    context_->tryGetComponent<StateLed>().value()->blue();
}

void ConfigState::onExit() {
    context_->tryGetComponent<StateLed>().value()->amber();

    ERROR_CHECK(context_->tryGetComponent<DnsServer>().value()->stop());
    ERROR_CHECK(context_->tryGetComponent<HttpServer>().value()->stop());
    ERROR_CHECK(context_->tryGetComponent<WifiAp>().value()->stop());
}

void ConfigState::toogleConfigMode() {
    context_->transit_state(NORMAL);
}

