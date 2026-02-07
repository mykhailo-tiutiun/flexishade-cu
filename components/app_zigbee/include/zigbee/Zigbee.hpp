#ifndef ZIGBEE_HPP
#define ZIGBEE_HPP

#include "rc/RcService.hpp"
#include <expected>
#include <string>
#include <thread>

#define ENDPOINT 1

/* Basic manufacturer information */
#define ESP_MANUFACTURER_NAME "\x09""ESPRESSIF"      /* Customized manufacturer name */
#define ESP_MODEL_IDENTIFIER "\x07"CONFIG_IDF_TARGET /* Customized model identifier */
#define ESP_ZB_PRIMARY_CHANNEL_MASK     (1l << 13)  /* Zigbee primary channel mask use in the example */

class Zigbee
{
    public:
        Zigbee(RcService* relay_service);
        std::expected<void, std::string> start();
        std::expected<void, std::string> stop();

        bool isUp() const;

    private:
        RcService* rc_service_;
        bool is_up_;
        std::optional<std::thread> zigbee_thread_;

        void zigbeeTask();
};

#endif
