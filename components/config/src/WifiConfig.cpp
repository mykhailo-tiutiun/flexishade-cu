#include "config/WifiConfig.hpp"
#include <utility>
#include "cJSON.h"
#include "config/ConfigError.hpp"

#define DEFUALT_SSID "My WiFi network"
#define DEFUALT_PASSWORD "1234568"

WifiConfig::WifiConfig()
: ssid_(DEFUALT_SSID)
, password_(DEFUALT_PASSWORD)
{}

cJSON* WifiConfig::getJson() const
{
    cJSON* json = cJSON_CreateObject();

    cJSON_AddStringToObject(json, "ssid", ssid_.c_str());
    cJSON_AddStringToObject(json, "password", password_.c_str());

    return json;
}

std::expected<void, ConfigError> WifiConfig::applyJson(const cJSON* json)
{
    cJSON* item;

    if ((item = cJSON_GetObjectItem(json, "ssid")) && cJSON_IsString(item)) {
        ssid_ = item->valuestring;
    } else {
        return std::unexpected(ConfigError::InvalidJson);
    }

    if ((item = cJSON_GetObjectItem(json, "password")) && cJSON_IsString(item)) {
        password_ = item->valuestring;
    } else {
        return std::unexpected(ConfigError::InvalidJson);
    }

    return {};
}

const char* WifiConfig::getName() const
{
    return "wifi";
}

const std::string& WifiConfig::getSSID() const
{
    return ssid_;
}

const std::string& WifiConfig::getPassword() const
{
    return password_;
}

