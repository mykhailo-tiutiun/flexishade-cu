#ifndef WIFI_STA_HPP
#define WIFI_STA_HPP

#define WIFI_STA_SSID "RaspAP"
#define WIFI_STA_PASS "45834110"
#define WIFI_STA_WIFI_CHANNEL 1
#define WIFI_STA_MAXIMUM_RETRY 3

#include "esp_wifi.h"
#include "esp_event.h"
#include <expected>
#include <memory>

#include "config/WifiConfig.hpp"

class WifiSta
{
    public:
        WifiSta();
        ~WifiSta();

        void configure(WifiConfig config);

        std::expected<void, std::string> start();
        std::expected<void, std::string> stop();

        bool isUp() const;

    private:
        WifiConfig config_;
        bool is_up_;
        esp_netif_t *eps_netif_;
        EventGroupHandle_t event_group_;
        int retry_counter_;
        esp_event_handler_instance_t event_any_id_;
        esp_event_handler_instance_t event_got_ip_;

        static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data);

        void retry_conn();
        void reset_retries();
};

#endif
