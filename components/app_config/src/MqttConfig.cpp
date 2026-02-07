#include "config/MqttConfig.hpp"

#define TOTAL_FIELDS 1
#define DEFUALT_URL ""

MqttConfig::MqttConfig()
: url_(DEFUALT_URL)
{}

cJSON* MqttConfig::getJson() const
{
    cJSON* json = cJSON_CreateObject();

    cJSON_AddStringToObject(json, "url", url_.c_str());

    return json;
}

std::expected<void, ConfigError> MqttConfig::applyJson(const cJSON* json)
{
    cJSON* item;

    if ((item = cJSON_GetObjectItem(json, "url")) && cJSON_IsString(item)) {
        url_ = item->valuestring;
    } else {
        return std::unexpected(ConfigError::InvalidJson);
    }

    return {};
}

const char* MqttConfig::getName() const
{
    return "mqtt";
}

const std::string& MqttConfig::getURL() const
{
    return url_;
}

