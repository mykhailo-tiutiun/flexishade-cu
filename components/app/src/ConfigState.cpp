#include "app/ConfigState.hpp"

#include "app/AppContext.hpp"
#include "config/ConfigStorage.hpp"
#include "esp_log.h"
#include "io/config/StateLed.hpp"
#include "io/config/WifiAp.hpp"
#include "io/config/HttpServer.hpp"
#include "io/config/dns_server.h"

static const char* TAG = "config state";

ConfigState::ConfigState() {}

void ConfigState::onEnter() {

    wifi_ap_ = new WifiAp();
    http_server_ = new HttpServer();
    dns_server_ = start_capture_portal_dns_server();

    context_->tryGetComponent<StateLed>().value()->blue();
}

void ConfigState::onExit() {
    stop_capture_portal_dns_server(dns_server_);

    delete http_server_;
    http_server_ = nullptr;

    delete wifi_ap_;
    wifi_ap_ = nullptr;

    context_->tryGetComponent<StateLed>().value()->amber();
}

void ConfigState::toogleConfigMode() {
    context_->transit_state(NORMAL);
}

