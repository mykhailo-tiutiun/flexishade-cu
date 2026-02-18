#include "wifi/WifiAp.hpp"
#include "esp_err.h"
#include "esp_wifi.h"
#include "esp_wifi_default.h"
#include <expected>
#include <string.h>
#include "esp_log.h"
#include "esp_wifi_types_generic.h"

#define ERROR_CHECK(exp, msg) if (exp != ESP_OK) { return std::unexpected(msg); }

static const char *TAG = "wifi_ap";

WifiAp::WifiAp()
: is_up_(false)
{}

WifiAp::~WifiAp() {
    stop();
}

std::expected<void, std::string> WifiAp::start()
{
    if (is_up_ == true) {
        return std::unexpected("WifiAP start, unexpected state: up");
    }

    netif_ = esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ERROR_CHECK(esp_wifi_init(&cfg), "Wifi AP init failed");

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_AP_SSID,
            .password = WIFI_AP_PASS,
            .ssid_len = strlen(WIFI_AP_SSID),
            .channel = WIFI_AP_WIFI_CHANNEL,
            .authmode = WIFI_AUTH_WPA2_PSK,
            .max_connection = WIFI_AP_MAX_STA_CONN,
            .pmf_cfg = {
                .required = true,
            },
            .bss_max_idle_cfg = {
                .period = WIFI_AP_DEFAULT_MAX_IDLE_PERIOD,
                .protected_keep_alive = 1,
            },
        },
    };
    if (strlen(WIFI_AP_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP), "Wifi AP init failed");

    ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config), "Wifi AP init failed");
    ERROR_CHECK(esp_wifi_start(), "Wifi AP init failed");

    ESP_LOGI(TAG, "Wifi AP started. SSID:%s password:%s channel:%d",
             WIFI_AP_SSID, WIFI_AP_PASS, WIFI_AP_WIFI_CHANNEL);

    is_up_ = true;

    return {};
}

std::expected<void, std::string> WifiAp::stop()
{
    if (is_up_ == false) {
        return std::unexpected("WifiAP stop, unexpected state: down");
    }

    is_up_ = false;

    ERROR_CHECK(esp_wifi_stop(), "Wifi AP stop failed");
    ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL), "Wifi AP stop failed");
    ERROR_CHECK(esp_wifi_deinit(), "Wifi AP stop failed");

    ERROR_CHECK(esp_wifi_clear_default_wifi_driver_and_handlers(netif_), "Wifi AP stop failed");

    esp_netif_destroy_default_wifi(netif_);

    ESP_LOGI(TAG, "Stopped");

    return {};
}

bool WifiAp::isUp() const {
    return is_up_;
}
