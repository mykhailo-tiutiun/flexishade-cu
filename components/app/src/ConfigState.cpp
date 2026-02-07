#include "app/ConfigState.hpp"

#include "app/AppContext.hpp"
#include "config/ConfigService.hpp"
#include "esp_log.h"
#include "io/config/StateLed.hpp"
#include "wifi/WifiAp.hpp"
#include "http/HttpServer.hpp"
#include "dns/DnsServer.hpp"

static const char* TAG = "config state";

ConfigState::ConfigState() {}

void ConfigState::onEnter() {

    context_->tryGetComponent<WifiAp>().value()->start();
    context_->tryGetComponent<HttpServer>().value()->start();
    context_->tryGetComponent<DnsServer>().value()->start();

    context_->tryGetComponent<StateLed>().value()->blue();
}

void ConfigState::onExit() {
    context_->tryGetComponent<StateLed>().value()->amber();

    context_->tryGetComponent<DnsServer>().value()->stop();
    context_->tryGetComponent<HttpServer>().value()->stop();
    context_->tryGetComponent<WifiAp>().value()->stop();
}

void ConfigState::toogleConfigMode() {
    context_->transit_state(NORMAL);
}

