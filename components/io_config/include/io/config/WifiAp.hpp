#ifndef WIFI_AP_HPP
#define WIFI_AP_HPP

#define WIFI_AP_SSID "ESP_TEST"
#define WIFI_AP_PASS "esp32pass"
#define WIFI_AP_WIFI_CHANNEL 1
#define WIFI_AP_MAX_STA_CONN 4

#if CONFIG_ESP_GTK_REKEYING_ENABLE
#define WIFI_AP_GTK_REKEY_INTERVAL CONFIG_ESP_GTK_REKEY_INTERVAL
#else
#define WIFI_AP_GTK_REKEY_INTERVAL 0
#endif

struct esp_netif_obj;

typedef struct esp_netif_obj esp_netif_t;

class WifiAp {
    private:
        bool is_up_;
        esp_netif_t* netif_;
    public:
        WifiAp();
        ~WifiAp();
        bool isUp() const;
};

#endif