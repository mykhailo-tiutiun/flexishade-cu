#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "cJSON.h"
#include "config/ConfigError.hpp"
#include "misc/Serializable.hpp"
#include <expected>
#include <memory>

class Config : public Serializable
{
    public:
        virtual ~Config() = default;
        virtual const char* getName() const = 0;

        virtual cJSON* getJson() const = 0;
        virtual std::expected<void, ConfigError> applyJson(const cJSON* json) = 0;

        virtual std::unique_ptr<Config> clone() const = 0;
};

#endif
