#ifndef NVS_CONFIG_STORAGE_HPP
#define NVS_CONFIG_STORAGE_HPP

#include "config/Config.hpp"

#include "config/ConfigError.hpp"
#include "config/ConfigStorage.hpp"
#include <memory>

class NvsConfigStorage : public ConfigStorage
{
    public:
        std::expected<void, ConfigError> loadAll() const override;

    protected:
        std::expected<void, ConfigError> load(std::shared_ptr<Config> config) const override;
        std::expected<void, ConfigError> save(std::shared_ptr<Config> config) const override;
};

#endif
