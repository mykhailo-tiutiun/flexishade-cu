#ifndef CONFIG_STORAGE_HPP
#define CONFIG_STORAGE_HPP

#include "config/Config.hpp"
#include "config/ConfigError.hpp"

#include "config/ConfigRepository.hpp"
#include "esp_log.h"
#include <map>
#include <memory>
#include <optional>

class ConfigService
{
    public:
        ConfigService(ConfigRepository* config_repository);

        template<typename T>
        std::optional<std::unique_ptr<T>> getConfig(const std::string& name) const
        {
            auto config_opt = getRawConfig(name);

            if (!config_opt) {
                return {};
            }

            auto config = std::move(config_opt.value());

            if (auto* raw = dynamic_cast<T*>(config.get())) {
                config.release();
                return std::unique_ptr<T>(raw);
            }

            ESP_LOGE("config_service", "Type mismatch for config '%s'", name.c_str());
            return {};
        }

        cJSON* exportAllToJson() const;
        std::expected<void, ConfigError> importJson(const std::string& name, const cJSON* json) const;
    private:
        std::optional<std::unique_ptr<Config>> getRawConfig(const std::string& name) const;
        ConfigRepository* config_repository_;
};

#endif
