#ifndef CONFIG_STORAGE_HPP
#define CONFIG_STORAGE_HPP

#include "config/Config.hpp"
#include "config/ConfigError.hpp"

#include "esp_log.h"
#include <map>
#include <memory>
#include <optional>

class ConfigService
{
    public:
        void registerConfig(std::shared_ptr<Config> config);

        template<typename T>
        std::optional<T> getConfig(const std::string& name) const
        {
            auto config_opt = getRawConfig(name);

            if (!config_opt.has_value()) {
                return std::nullopt;
            }

            auto config = config_opt.value();

            auto casted = std::dynamic_pointer_cast<T>(config);
            if (!casted) {
                ESP_LOGE("config manager", "Type mismatch for config '%s'", name.c_str());
                return {};
            }

            return *casted;
        }

        virtual std::expected<void, ConfigError> loadAll() const = 0;

        cJSON* exportAllToJson() const;
        std::expected<void, ConfigError> importJson(const std::string& name, const cJSON* json) const;

    protected:
        std::optional<std::shared_ptr<Config>> getRawConfig(const std::string& name) const;

        virtual std::expected<void, ConfigError> load(std::shared_ptr<Config>) const = 0;
        virtual std::expected<void, ConfigError> save(std::shared_ptr<Config> config) const = 0;

        std::map<std::string, std::shared_ptr<Config>> configs_;
};

#endif
