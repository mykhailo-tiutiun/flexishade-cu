#include "nvs/NvsConfigRepository.hpp"
#include "nvs/Nvs.hpp"
#include "config/ConfigRepository.hpp"
#include "config/Config.hpp"
#include "misc/Error.hpp"
#include <memory>
#include <string>

NvsConfigRepository::NvsConfigRepository(Nvs* nvs)
: nvs_(nvs)
{}


void NvsConfigRepository::registerConfig(std::unique_ptr<Config> config)
{
    config_templates_[config->getName()] = std::move(config);
}

std::expected<std::unique_ptr<Config>, Error<ConfigRepositoryError>> NvsConfigRepository::getConfigTemplate(const std::string& name) const
{
    auto conf_it = config_templates_.find(name);
    if (conf_it == config_templates_.end()) {
        return make_error(ConfigRepositoryError::Other, "Config template is not registered");
    }

    return conf_it->second->clone();
}

std::expected<std::unique_ptr<Config>, Error<ConfigRepositoryError>> NvsConfigRepository::getByName(const std::string& name) const
{
    auto config_template_res = getConfigTemplate(name);
    if (!config_template_res) {
        return config_template_res;
    }
    auto config_template = std::move(*config_template_res);

    auto nvs_id = getNvsKeyByConfigName(name);

    std::vector<std::uint8_t> buf(config_template->serializationSize());

    auto data_res = nvs_->load(nvs_id, buf);
    if (!data_res) {
        auto err = data_res.error();
        if (err.kind() == NvsError::NotFound) {
            return make_error(ConfigRepositoryError::NotFound, err);
        } else {
            return make_error(ConfigRepositoryError::Other, err);
        }
    }

    auto desirialize_res = config_template->desirialize(buf);
    if (!desirialize_res) {
        return make_error(ConfigRepositoryError::Other, desirialize_res.error());
    }

    return config_template;
}

std::expected<std::vector<std::unique_ptr<Config>>, Error<ConfigRepositoryError>> NvsConfigRepository::getAll() const
{
    return make_error(ConfigRepositoryError::Other, "not implemented");
}

std::expected<void, Error<ConfigRepositoryError>> NvsConfigRepository::save(std::unique_ptr<Config> config) const
{
    auto nvs_id = getNvsKeyByConfigName(config->getName());

    auto data = config->serialize();

    auto res = nvs_->save(nvs_id, data);

    if (!res) {
        return make_error(ConfigRepositoryError::Other, res.error());
    }

    return {};
}

std::expected<void, Error<ConfigRepositoryError>> NvsConfigRepository::reset() const
{
    return make_error(ConfigRepositoryError::Other, "not implemented");
}

std::string NvsConfigRepository::getNvsKeyByConfigName(const std::string& name) const
{
    return std::format("{}{}", NVS_KEY_PREFIX, name);
}
