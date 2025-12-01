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

