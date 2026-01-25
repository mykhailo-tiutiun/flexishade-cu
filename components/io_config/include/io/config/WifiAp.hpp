#ifndef WIFI_AP_HPP
#define WIFI_AP_HPP

#define WIFI_AP_SSID "CU-Config"
#define WIFI_AP_PASS "12345678"
#define WIFI_AP_WIFI_CHANNEL 1
#define WIFI_AP_MAX_STA_CONN 4

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
