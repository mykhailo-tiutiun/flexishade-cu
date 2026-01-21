#include "config/ConfigService.hpp"
#include "cJSON.h"
#include "config/Config.hpp"
#include "config/ConfigError.hpp"
#include "esp_log.h"
#include <memory>
#include <optional>
#include <utility>

static const char* TAG = "config_storage";

void ConfigService::registerConfig(std::shared_ptr<Config> config)
{
    configs_[config->getName()] = config;
    load(config);
}

cJSON* ConfigService::exportAllToJson() const
{
    cJSON* root = cJSON_CreateArray();
    for (auto& [key, config] : configs_) {
        cJSON* array_item = cJSON_CreateObject();
        cJSON* config_json = config->getJson();


        cJSON_AddStringToObject(array_item, "name", config->getName());
        cJSON_AddItemToObject(array_item, "config", config_json);

        cJSON_AddItemToArray(root, array_item);
    }
    return root;
}

std::expected<void, ConfigError> ConfigService::importJson(const std::string& name, const cJSON* json) const
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

std::optional<std::shared_ptr<Config>> ConfigService::getRawConfig(const std::string& name) const
{
    auto it = configs_.find(name);
    if (it == configs_.end()) {
        ESP_LOGE("config manager", "Config not found '%s'", name.c_str());
        return {};
    }
    return it->second;
}
