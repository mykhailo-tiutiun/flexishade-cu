#ifndef MQTT_CONFIG_HPP
#define MQTT_CONFIG_HPP

#include "config/Config.hpp"
#include "config/ConfigError.hpp"
#include "cJSON.h"

#include <string>

class MqttConfig : public Config
{
    public:
        MqttConfig();

        cJSON* getJson() const override;
        std::expected<void, ConfigError> applyJson(const cJSON* json) override;
        const char* getName() const override;

        const std::string& getURL() const;

    private:
        std::string url_;
};
#endif
