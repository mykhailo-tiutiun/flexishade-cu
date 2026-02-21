#ifndef WIFI_STA_CONFIG_HPP
#define WIFI_STA_CONFIG_HPP

#include "cJSON.h"
#include "config/Config.hpp"
#include "config/ConfigError.hpp"
#include "misc/Error.hpp"
#include "misc/Serializable.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

class WifiStaConfig : public Config
{
    public:
        WifiStaConfig();
        WifiStaConfig(std::string ssid, std::string psk);
        WifiStaConfig(const WifiStaConfig&) = default;

        static constexpr std::size_t WIFI_SSID_LENGTH = 32;
        static constexpr std::size_t WIFI_PSK_LENGTH  = 63;

        cJSON* getJson() const override;
        std::expected<void, ConfigError> applyJson(const cJSON* json) override;

        const char* getName() const override
        {
            return "wifi";
        }

        const std::string& getSSID() const;
        const std::string& getPsk() const;

        std::vector<std::uint8_t> serialize() const override;
        std::expected<void, Error<SerializationError>> desirialize(const std::vector<std::uint8_t>& data) override;

        std::size_t serializationSize() const override
        {
            return WIFI_SSID_LENGTH + WIFI_PSK_LENGTH;
        }

        std::unique_ptr<Config> clone() const override;

    private:
        std::string ssid_;
        std::string psk_;
};

#endif
