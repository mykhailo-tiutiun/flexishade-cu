#ifndef NVS_CONFIG_REPOSITORY_HPP
#define NVS_CONFIG_REPOSITORY_HPP

#include "config/Config.hpp"
#include "misc/Error.hpp"
#include "nvs/Nvs.hpp"
#include "config/Config.hpp"
#include "config/ConfigRepository.hpp"
#include <expected>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#define NVS_KEY_PREFIX "config_"

class NvsConfigRepository : public ConfigRepository {
    public:
        NvsConfigRepository(Nvs* nvs);

        void registerConfig(std::unique_ptr<Config> config);

        std::expected<std::unique_ptr<Config>, Error<ConfigRepositoryError>> getByName(const std::string& name) const override;
        std::expected<std::vector<std::unique_ptr<Config>>, Error<ConfigRepositoryError>> getAll() const override;

        std::expected<void, Error<ConfigRepositoryError>> save(std::unique_ptr<Config> relay) const override;
        std::expected<void, Error<ConfigRepositoryError>> reset() const override;

    private:
        Nvs* nvs_;
        std::unordered_map<std::string, std::unique_ptr<Config>> config_templates_;

        std::string getNvsKeyByConfigName(const std::string& name) const;
        std::expected<std::unique_ptr<Config>, Error<ConfigRepositoryError>> getConfigTemplate(const std::string& name) const;
};

#endif
