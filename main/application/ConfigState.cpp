#include "ConfigState.hpp"
#include "../StateLed.hpp"
#include "AppContext.hpp"
#include "../net/wifi/WifiAp.hpp"
#include "../http/HttpServer.hpp"
#include "../net/dns_server.h"

ConfigState::ConfigState() {}

void ConfigState::onEnter() {

    wifi_ap_ = new WifiAp();
    http_server_ = new HttpServer();
    dns_server_ = start_capture_portal_dns_server();

    context_->getStateLed()->blue();
}

void ConfigState::onExit() {
    stop_capture_portal_dns_server(dns_server_);

    delete http_server_;
    http_server_ = nullptr;

    delete wifi_ap_;
    wifi_ap_ = nullptr;

    context_->getStateLed()->amber();

    vTaskDelay(pdMS_TO_TICKS(500)); 
}

void ConfigState::toogleConfigMode() {
    AppState* next_state = context_->getAppStateFactory()->normal();
    context_->transit_state(next_state);
}

const char* ConfigState::getName() const {
    return "Config";
}