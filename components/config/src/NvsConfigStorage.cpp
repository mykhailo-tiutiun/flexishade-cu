#include "config/NvsConfigStorage.hpp"

#include "cJSON.h"
#include "config/Config.hpp"
#include "config/ConfigError.hpp"

#include "nvs.h"
#include <expected>
#include <memory>
#include <vector>

static const char* TAG = "nvs config manager";

std::expected<void, ConfigError> NvsConfigStorage::load(std::shared_ptr<Config> config) const
{
    esp_err_t err;
    nvs_handle_t handle;

    auto name = config->getName();

    err = nvs_open("storage", NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS: %s", esp_err_to_name(err));
        return std::unexpected(ConfigError::NvsOpenFailed);
    }

    std::vector<char> buf(4000);
    size_t required_size = buf.size();
    err = nvs_get_str(handle, name, buf.data(), &required_size);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGW(TAG, "Config not found: %s", name);
        nvs_close(handle);
        return std::unexpected(ConfigError::NvsConfigNotFound);
    }

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to load config %s: %s", name, esp_err_to_name(err));
        nvs_close(handle);
        return std::unexpected(ConfigError::NvsLoadFailed);
    }

    cJSON* json = cJSON_Parse(buf.data());
    if (json) {
        config->applyJson(json);
        cJSON_Delete(json);
    } else {
        ESP_LOGE(TAG, "Invalid json %s: %s", name, esp_err_to_name(err));
        nvs_close(handle);
        return std::unexpected(ConfigError::InvalidJson);
    }

    nvs_close(handle);

    return {};
}

std::expected<void, ConfigError> NvsConfigStorage::loadAll() const
{
    esp_err_t err;
    nvs_handle_t handle;

    err = nvs_open("storage", NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS: %s", esp_err_to_name(err));
        return std::unexpected(ConfigError::NvsOpenFailed);
    }

    std::vector<char> buf(4000);

    for (auto& [key, config] : configs_) {
        size_t required_size = buf.size();

        auto name = config->getName();

        err = nvs_get_str(handle, name, buf.data(), &required_size);
        if (err == ESP_ERR_NVS_NOT_FOUND) {
            ESP_LOGW(TAG, "Config not found: %s", name);
            nvs_close(handle);
            return std::unexpected(ConfigError::NvsConfigNotFound);
        }

        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to load config %s: %s", name, esp_err_to_name(err));
            nvs_close(handle);
            return std::unexpected(ConfigError::NvsLoadFailed);
        }

        cJSON* json = cJSON_Parse(buf.data());
        if (json) {
            config->applyJson(json);
            cJSON_Delete(json);
        } else {
            ESP_LOGE(TAG, "Invalid json %s: %s", name, esp_err_to_name(err));
            nvs_close(handle);
            return std::unexpected(ConfigError::InvalidJson);
        }
    }

    nvs_close(handle);

    return {};
}

std::expected<void, ConfigError> NvsConfigStorage::save(std::shared_ptr<Config> config) const
{
    esp_err_t err;
    nvs_handle_t handle;

    err = nvs_open("storage", NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS: %s", esp_err_to_name(err));
        return std::unexpected(ConfigError::NvsOpenFailed);
    }

    auto name = config->getName();
    auto json = config->getJson();
    auto json_str = cJSON_PrintUnformatted(json);

    err = nvs_set_str(handle, name, json_str);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to save %s: %s", name, esp_err_to_name(err));
        nvs_close(handle);
        return std::unexpected(ConfigError::NvsSaveFailed);
    }

    err = nvs_commit(handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to commit: %s", esp_err_to_name(err));
        nvs_close(handle);
        return std::unexpected(ConfigError::NvsCommitFailed);
    }

    nvs_close(handle);

    return {};
}
