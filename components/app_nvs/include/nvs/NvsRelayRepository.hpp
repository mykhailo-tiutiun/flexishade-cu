#ifndef NVS_RELAY_REPOSITORY_HPP
#define NVS_RELAY_REPOSITORY_HPP

#include "misc/Error.hpp"
#include "nvs/Nvs.hpp"
#include "relay/Relay.hpp"
#include "relay/RelayRepository.hpp"
#include <expected>
#include <vector>

#define NVS_KEY_PREFIX "relay_"

class NvsRelayRepository : public RelayRepository {
    public:
        NvsRelayRepository(Nvs* nvs);

        std::expected<Relay, Error<RelayRepositoryError>> getById(RelayId id) const override;
        std::expected<std::vector<Relay>, Error<RelayRepositoryError>> getAll() const override;

        std::expected<void, Error<RelayRepositoryError>> save(Relay relay) const override;
        std::expected<void, Error<RelayRepositoryError>> reset() const override;

    private:
        Nvs* nvs_;

        std::string getNvsKeyByRelayId(RelayId id) const;
};

#endif
