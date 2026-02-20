#ifndef CONFIG_REPOSITORY_HPP
#define CONFIG_REPOSITORY_HPP

#include "config/Config.hpp"
#include "misc/Error.hpp"
#include <memory>

enum class ConfigRepositoryError {
    NotFound,
    Other
};

class ConfigRepository {
    public:
        virtual std::expected<std::unique_ptr<Config>, Error<ConfigRepositoryError>> getByName(const std::string& name) const = 0;
        virtual std::expected<std::vector<std::unique_ptr<Config>>, Error<ConfigRepositoryError>> getAll() const = 0;

        virtual std::expected<void, Error<ConfigRepositoryError>> save(std::unique_ptr<Config> config) const = 0;
        virtual std::expected<void, Error<ConfigRepositoryError>> reset() const = 0;
};

#endif
