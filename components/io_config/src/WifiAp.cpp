#include "io/config/WifiAp.hpp"
#include "esp_wifi.h"
#include "esp_wifi_default.h"
#include <string.h>
#include "esp_log.h"
#include "esp_wifi_types_generic.h"

static const char *TAG = "wifi softAP";

WifiAp::WifiAp() : is_up_(false) {
    netif_ = esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_AP_SSID,
            .password = WIFI_AP_PASS,
            .ssid_len = strlen(WIFI_AP_SSID),
            .channel = WIFI_AP_WIFI_CHANNEL,
#ifdef CONFIG_ESP_WIFI_WIFIAP_SAE_SUPPORT
            .authmode = WIFI_AUTH_WPA3_PSK,
#else
            .authmode = WIFI_AUTH_WPA2_PSK,
#endif
            .max_connection = WIFI_AP_MAX_STA_CONN,
            .pmf_cfg = {
                    .required = true,
            },
#ifdef CONFIG_ESP_WIFI_WIFIAP_SAE_SUPPORT
            .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
#endif
#ifdef CONFIG_ESP_WIFI_BSS_MAX_IDLE_SUPPORT
            .bss_max_idle_cfg = {
                .period = WIFI_AP_DEFAULT_MAX_IDLE_PERIOD,
                .protected_keep_alive = 1,
            },
#endif
            .gtk_rekey_interval = WIFI_AP_GTK_REKEY_INTERVAL,
        },
    };
    if (strlen(WIFI_AP_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             WIFI_AP_SSID, WIFI_AP_PASS, WIFI_AP_WIFI_CHANNEL);

    is_up_ = true;
}

WifiAp::~WifiAp() {
    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
    ESP_ERROR_CHECK(esp_wifi_deinit());

    ESP_ERROR_CHECK(esp_wifi_clear_default_wifi_driver_and_handlers(
 netif_));

    esp_netif_destroy_default_wifi(netif_);
    is_up_ = false;
}

bool WifiAp::isUp() const {
    return is_up_;
}
