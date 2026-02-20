#ifndef RELAY_REPOSITORY_HPP
#define RELAY_REPOSITORY_HPP

#include "misc/Error.hpp"
#include "relay/Relay.hpp"
#include <expected>
#include <vector>

enum class RelayRepositoryError {
    NotFound,
    Other
};

class RelayRepository {
    public:
        virtual std::expected<Relay, Error<RelayRepositoryError>> getById(RelayId id) const = 0;
        virtual std::expected<std::vector<Relay>, Error<RelayRepositoryError>> getAll() const = 0;

        virtual std::expected<void, Error<RelayRepositoryError>> save(Relay relay) const = 0;
        virtual std::expected<void, Error<RelayRepositoryError>> reset() const = 0;
};

#endif
