#include "config/ConfigStorage.hpp"
#include "config/Config.hpp"
#include "config/ConfigError.hpp"
#include "esp_log.h"
#include <memory>
#include <optional>
#include <utility>

static const char* TAG = "config storage";

void ConfigStorage::registerConfig(std::shared_ptr<Config> config)
{
    configs_[config->getName()] = config;
    load(config);
}

cJSON* ConfigStorage::exportAllToJson() const
{
    cJSON* root = cJSON_CreateObject();
    for (auto& [key, config] : configs_) {
        cJSON* configJson = config->getJson();
        cJSON_AddItemToObject(root, config->getName(), configJson);
    }
    return root;
}

std::expected<void, ConfigError> ConfigStorage::importJson(const std::string& name, const cJSON* json) const
{
    auto config = getRawConfig(name);
    if (!config) {
        return std::unexpected(ConfigError::ConfigNotFound);
    }

    auto cfg = config.value();
    cfg->applyJson(json);
    save(cfg);

    return {};
}

std::optional<std::shared_ptr<Config>> ConfigStorage::getRawConfig(const std::string& name) const
{
    auto it = configs_.find(name);
    if (it == configs_.end()) {
        ESP_LOGE("config manager", "Config not found '%s'", name.c_str());
        return {};
    }
    return it->second;
}
