#include "config/WifiConfig.hpp"
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "cJSON.h"
#include "config/ConfigError.hpp"
#include "misc/Error.hpp"
#include "misc/Serializable.hpp"

WifiConfig::WifiConfig()
: ssid_("")
, psk_("")
{}

WifiConfig::WifiConfig(std::string ssid, std::string psk)
: ssid_(ssid)
, psk_(psk)
{}

cJSON* WifiConfig::getJson() const
{
    cJSON* json = cJSON_CreateObject();

    cJSON_AddStringToObject(json, "ssid", ssid_.c_str());
    cJSON_AddStringToObject(json, "psk", psk_.c_str());

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

    if ((item = cJSON_GetObjectItem(json, "psk")) && cJSON_IsString(item)) {
        psk_ = item->valuestring;
    } else {
        return std::unexpected(ConfigError::InvalidJson);
    }

    return {};
}

const std::string& WifiConfig::getSSID() const
{
    return ssid_;
}

const std::string& WifiConfig::getPsk() const
{
    return psk_;
}

std::vector<std::uint8_t> WifiConfig::serialize() const
{
    std::vector<std::uint8_t> buf;
    buf.reserve(serializationSize());

    buf.insert(buf.end(), ssid_.begin(), ssid_.end());
    buf.insert(buf.end(), WIFI_SSID_LENGTH - ssid_.size(), static_cast<std::uint8_t>(0));

    buf.insert(buf.end(), psk_.begin(), psk_.end());
    buf.insert(buf.end(), WIFI_PSK_LENGTH - psk_.size(), static_cast<std::uint8_t>(0));

    return buf;
}

std::expected<void, Error<SerializationError>> WifiConfig::desirialize(const std::vector<std::uint8_t>& data)
{
    if (data.size() != serializationSize()) {
        return make_error(SerializationError::InvalidDataLength, "WifiConfig desirialization: invalid data length");
    }

    std::size_t off = 0;

    size_t ssid_len = 0;
    while (ssid_len < WIFI_SSID_LENGTH && data[ssid_len + off] != 0) ++ssid_len;
    ssid_.assign(reinterpret_cast<const char*>(data.data() + off), ssid_len);
    off += WIFI_SSID_LENGTH;

    size_t psk_len = 0;
    while (psk_len < WIFI_PSK_LENGTH && data[psk_len + off] != 0) ++psk_len;
    psk_.assign(reinterpret_cast<const char*>(data.data() + off), psk_len);
    off += WIFI_PSK_LENGTH;

    return {};
}

std::unique_ptr<Config> WifiConfig::clone() const
{
    return std::make_unique<WifiConfig>(*this);
}
