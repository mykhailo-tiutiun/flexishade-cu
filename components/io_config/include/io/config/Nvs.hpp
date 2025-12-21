#ifndef NVS_HPP
#define NVS_HPP

#include <expected>
#include <utility>
#include <vector>
#include <string>

enum NvsError {
    NotFound,
    Other,
};

using NvsData = std::vector<std::pair<std::string, std::string>>;
using NvsKeys = std::vector<std::string>;

class Nvs
{
    public:
        std::expected<NvsData, NvsError> load(const NvsKeys& keys) const;
        std::expected<void, NvsError> save(const NvsData& data) const;
};

#endif
