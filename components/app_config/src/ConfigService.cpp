#include "config/ConfigService.hpp"
#include "cJSON.h"
#include "config/Config.hpp"
#include "config/ConfigError.hpp"
#include "config/ConfigRepository.hpp"
#include "esp_log.h"
#include <memory>
#include <optional>
#include <utility>

static const char* TAG = "config_service";

ConfigService::ConfigService(ConfigRepository* config_repository)
: config_repository_(config_repository)
{}

cJSON* ConfigService::exportAllToJson() const
{
    cJSON* root = cJSON_CreateArray();
//    for (auto& [key, config] : configs_) {
//        cJSON* array_item = cJSON_CreateObject();
//        cJSON* config_json = config->getJson();
//
//
//        cJSON_AddStringToObject(array_item, "name", config->getName());
//        cJSON_AddItemToObject(array_item, "config", config_json);
//
//        cJSON_AddItemToArray(root, array_item);
//    }
    return root;
}

std::expected<void, ConfigError> ConfigService::importJson(const std::string& name, const cJSON* json) const
{
    auto config = getRawConfig(name);
    if (!config) {
        return std::unexpected(ConfigError::ConfigNotFound);
    }

    auto cfg = std::move(config.value());
    cfg->applyJson(json);
    config_repository_->save(std::move(cfg));

    return {};
}

std::optional<std::unique_ptr<Config>> ConfigService::getRawConfig(const std::string& name) const
{
    auto conf_res = config_repository_->getByName(name);

    if (!conf_res) {
        auto err = conf_res.error();
        if (err.kind() == ConfigRepositoryError::NotFound) {
            ESP_LOGW(TAG, "Config not found: %s", err.msg().c_str());
            return {};
        } else {
            ESP_LOGE(TAG, "Config get, unexpected error: %s", err.msg().c_str());
            return {};
        }
    }

    return std::make_optional(std::move(*conf_res));
}
