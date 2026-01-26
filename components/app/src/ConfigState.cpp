#include "app/ConfigState.hpp"

#include "app/AppContext.hpp"
#include "config/ConfigService.hpp"
#include "esp_log.h"
#include "io/config/StateLed.hpp"
#include "io/config/WifiAp.hpp"
#include "io/config/HttpServer.hpp"
#include "io/config/dns_server.h"

static const char* TAG = "config state";

ConfigState::ConfigState() {}

void ConfigState::onEnter() {

    context_->tryGetComponent<WifiAp>().value()->start();
    context_->tryGetComponent<HttpServer>().value()->start();
    dns_server_ = start_capture_portal_dns_server();

    context_->tryGetComponent<StateLed>().value()->blue();
}

void ConfigState::onExit() {
    stop_capture_portal_dns_server(dns_server_);

    context_->tryGetComponent<HttpServer>().value()->stop();
    context_->tryGetComponent<WifiAp>().value()->stop();

    context_->tryGetComponent<StateLed>().value()->amber();
}

void ConfigState::toogleConfigMode() {
    context_->transit_state(NORMAL);
}

