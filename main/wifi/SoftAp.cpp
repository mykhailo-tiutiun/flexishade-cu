#include "SoftAp.hpp"
#include "esp_wifi.h"
#include "esp_wifi_default.h"
#include <string.h>
#include "esp_log.h"
#include "esp_wifi_types_generic.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"

static const char *TAG = "wifi softAP";

SoftAp::SoftAp() : is_up_(false) {
    netif_ = esp_netif_create_default_wifi_ap();

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = SOFT_AP_SSID,
            .password = SOFT_AP_PASS,
            .ssid_len = strlen(SOFT_AP_SSID),
            .channel = SOFT_AP_WIFI_CHANNEL,
#ifdef CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT
            .authmode = WIFI_AUTH_WPA3_PSK,
#else
            .authmode = WIFI_AUTH_WPA2_PSK,
#endif
            .max_connection = SOFT_AP_MAX_STA_CONN,
            .pmf_cfg = {
                    .required = true,
            },
#ifdef CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT
            .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
#endif
#ifdef CONFIG_ESP_WIFI_BSS_MAX_IDLE_SUPPORT
            .bss_max_idle_cfg = {
                .period = WIFI_AP_DEFAULT_MAX_IDLE_PERIOD,
                .protected_keep_alive = 1,
            },
#endif
            .gtk_rekey_interval = SOFT_AP_GTK_REKEY_INTERVAL,
        },
    };
    if (strlen(SOFT_AP_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             SOFT_AP_SSID, SOFT_AP_PASS, SOFT_AP_WIFI_CHANNEL);
    is_up_ = true;
}

SoftAp::~SoftAp() {
    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
    esp_netif_destroy_default_wifi(netif_);
    is_up_ = false;
}

bool SoftAp::isUp() const {
    return is_up_;
}