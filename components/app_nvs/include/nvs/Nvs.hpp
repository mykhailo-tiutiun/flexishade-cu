#ifndef NVS_HPP
#define NVS_HPP

#include "misc/Error.hpp"
#include <cstdint>
#include <expected>
#include <string>
#include <vector>

enum class NvsError {
    NotFound,
    Other
};

using NvsKey = std::string;
using NvsData = std::vector<std::uint8_t>;

class Nvs
{
    public:
        std::expected<void, Error<NvsError>> load(const NvsKey& key, NvsData& buf) const;
        std::expected<std::vector<NvsData>, Error<NvsError>> loadMany(const std::vector<NvsKey>& keys) const;
        std::expected<void, Error<NvsError>> save(const NvsKey& key, NvsData data) const;
};

#endif
