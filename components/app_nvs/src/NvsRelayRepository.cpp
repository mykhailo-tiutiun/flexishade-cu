#include "nvs/NvsRelayRepository.hpp"
#include "relay/Relay.hpp"
#include "misc/Error.hpp"
#include <string>
#include <format>
#include <expected>
#include <string_view>
#include "esp_log.h"

NvsRelayRepository::NvsRelayRepository(Nvs* nvs)
: nvs_(nvs)
{}

std::expected<Relay, Error<RelayRepositoryError>> NvsRelayRepository::getById(RelayId id) const
{
    auto nvs_id = getNvsKeyByRelayId(id);

    std::vector<std::uint8_t> buf(Relay::serializationSize());

    auto data_res = nvs_->load(nvs_id, buf);
    if (!data_res) {
        auto err = data_res.error();
        if (err.kind() == NvsError::NotFound) {
            return make_error(RelayRepositoryError::NotFound, err);
        } else {
            return make_error(RelayRepositoryError::Other, err);
        }
    }

    auto relay_res = Relay::desirialize(buf);
    if (!relay_res) {
        return make_error(RelayRepositoryError::Other, relay_res.error());
    }

    return *relay_res;
}

std::expected<std::vector<Relay>, Error<RelayRepositoryError>> NvsRelayRepository::getAll() const
{
    return make_error(RelayRepositoryError::Other, "not implemented");
}

std::expected<void, Error<RelayRepositoryError>> NvsRelayRepository::save(Relay relay) const
{
    auto nvs_id = getNvsKeyByRelayId(relay.getId());

    auto data = relay.serialize();

    auto res = nvs_->save(nvs_id, data);

    if (!res) {
        return make_error(RelayRepositoryError::Other, res.error());
    }

    return {};
}

std::expected<void, Error<RelayRepositoryError>> NvsRelayRepository::reset() const
{
    return make_error(RelayRepositoryError::Other, "not implemented");
}

std::string NvsRelayRepository::getNvsKeyByRelayId(RelayId id) const
{
    auto relay_id_str = id.asAscii85();

    return std::format("{}{}", NVS_KEY_PREFIX, relay_id_str);
}
