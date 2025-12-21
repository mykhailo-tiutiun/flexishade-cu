#ifndef WIFI_CONFIG_HPP
#define WIFI_CONFIG_HPP

#include "cJSON.h"
#include "config/Config.hpp"
#include "config/ConfigError.hpp"
#include <string>

class WifiConfig : public Config
{
    public:
        WifiConfig();

        cJSON* getJson() const override;
        std::expected<void, ConfigError> applyJson(const cJSON* json) override;
        const char* getName() const override;

        const std::string& getSSID() const;
        const std::string& getPassword() const;
    private:
        std::string ssid_;
        std::string password_;
};

#endif
