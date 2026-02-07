#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "cJSON.h"
#include "config/ConfigError.hpp"
#include <concepts>
#include <expected>
#include <string>
#include <vector>
#include <utility>


class Config
{
    public:
        virtual ~Config() = default;

        virtual cJSON* getJson() const = 0;
        virtual std::expected<void, ConfigError> applyJson(const cJSON* json) = 0;
        virtual const char* getName() const = 0;
};

#endif
