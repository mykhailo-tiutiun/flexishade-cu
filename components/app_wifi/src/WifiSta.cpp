#include "wifi/WifiSta.hpp"
#include "esp_event.h"
#include "esp_event_base.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"

#include <cstdint>
#include <expected>
#include <cstring>
#include <string>
#include <utility>

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

#define ERROR_CHECK(exp, msg) if (exp != ESP_OK) { return std::unexpected(msg); }

static const char *TAG = "wifi sta";

void WifiSta::event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    WifiSta* wifi_sta = (WifiSta*) arg;
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        wifi_sta->retry_conn();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        wifi_sta->reset_retries();
        xEventGroupSetBits(wifi_sta->event_group_, WIFI_CONNECTED_BIT);
    }
}

WifiSta::WifiSta() : is_up_(false)
{

}

WifiSta::~WifiSta()
{
    if (isUp()) {
        stop();
    }
}

void WifiSta::configure(std::unique_ptr<WifiConfig> config)
{
    config_ = std::move(config);
}

std::expected<void, std::string> WifiSta::start()
{
    if (is_up_ == true) {
        return std::unexpected("WifiSta start, unexpected state: up");
    }

    event_group_ = xEventGroupCreate();
    eps_netif_ = esp_netif_create_default_wifi_sta();
    retry_counter_ = 0;

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ERROR_CHECK(esp_wifi_init(&cfg), "Wifi Sta init failed");


    ERROR_CHECK(
        esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, (void *) this, &event_any_id_),
        "Wifi Sta init failed"
    );

    ERROR_CHECK(
        esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, (void *) this, &event_got_ip_),
        "Wifi Sta init failed"
    );

    wifi_config_t wifi_config = {
        .sta = {
            .threshold = {
                .authmode = WIFI_AUTH_WPA2_PSK,
            },
            .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
            .sae_h2e_identifier = "",
        },
    };

    std::strncpy((char*)(&wifi_config.sta.ssid), config_->getSSID().c_str(), 32);
    std::strncpy((char*)(&wifi_config.sta.password), config_->getPsk().c_str(), 64);


    ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA), "Wifi Sta init failed");
    ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config), "Wifi Sta init failed");
    ERROR_CHECK(esp_wifi_start(), "Wifi Sta init failed");

    is_up_ = true;

    EventBits_t bits = xEventGroupWaitBits(event_group_,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 config_->getSSID().c_str(), config_->getPsk().c_str());
        return {};
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGE(TAG, "Failed to connect to SSID:%s, password:%s",
                 config_->getSSID().c_str(), config_->getPsk().c_str());
        stop();
        return std::unexpected("Wifi Sta connection failed");
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
        stop();
        return std::unexpected("Wifi Sta connection failed");
    }
}

std::expected<void, std::string> WifiSta::stop()
{
    if (is_up_ == false) {
        return std::unexpected("WifiSta stop, unexpected state: down");
    }
    is_up_ = false;

    ERROR_CHECK(
        esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, event_any_id_),
        "Wifi Sta stop failed"
    );
    ERROR_CHECK(
        esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, event_got_ip_),
        "Wifi Sta stop failed"
    );

    ERROR_CHECK(esp_wifi_disconnect(), "Wifi Sta stop failed");
    ERROR_CHECK(esp_wifi_stop(), "Wifi Sta stop failed");
    ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL), "Wifi Sta stop failed");

    esp_netif_destroy_default_wifi(eps_netif_);

    ERROR_CHECK(esp_wifi_deinit(), "Wifi Sta stop failed");

    vEventGroupDelete(event_group_);

    return {};
}

void WifiSta::retry_conn()
{
    if (retry_counter_ < WIFI_STA_MAXIMUM_RETRY) {
        esp_wifi_connect();
        retry_counter_++;
        ESP_LOGI(TAG, "retry to connect to the AP");
    } else {
        xEventGroupSetBits(event_group_, WIFI_FAIL_BIT);
    }
    ESP_LOGI(TAG,"connect to the AP fail");
}

void WifiSta::reset_retries()
{
    retry_counter_ = 0;
}

bool WifiSta::isUp() const
{
    return is_up_;
}
