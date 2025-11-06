#ifndef SOFT_AP_HPP
#define SOFT_AP_HPP

#define SOFT_AP_SSID "ESP_TEST"
#define SOFT_AP_PASS "esp32pass"
#define SOFT_AP_WIFI_CHANNEL 1
#define SOFT_AP_MAX_STA_CONN 4

#if CONFIG_ESP_GTK_REKEYING_ENABLE
#define SOFT_AP_GTK_REKEY_INTERVAL CONFIG_ESP_GTK_REKEY_INTERVAL
#else
#define SOFT_AP_GTK_REKEY_INTERVAL 0
#endif

struct esp_netif_obj;

typedef struct esp_netif_obj esp_netif_t;

class SoftAp {
    private:
        bool is_up_;
        esp_netif_t* netif_;
    public:
        SoftAp();
        ~SoftAp();
        bool isUp() const;
};

#endif