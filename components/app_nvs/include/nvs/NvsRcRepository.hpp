#ifndef NVS_RC_REPOSITORY_HPP
#define NVS_RC_REPOSITORY_HPP

#include "nvs/Nvs.hpp"
#include "rc/Rc.hpp"
#include "rc/RcRepository.hpp"
#include "misc/Error.hpp"
#include <optional>
#include <expected>
#include <string>
#include <string_view>

#define NVS_KEY_PREFIX "rc_"

class NvsRcRepository : public RcRepository {
    public:
        NvsRcRepository(Nvs* nvs);

        std::expected<Rc, Error<RcRepositoryError>> getById(RcId id) const override;
        std::expected<std::vector<Rc>, Error<RcRepositoryError>> getAll() const override;

        std::expected<void, Error<RcRepositoryError>> save(Rc rc) const override;
        std::expected<void, Error<RcRepositoryError>> reset() const override;

    private:
        Nvs* nvs_;

        std::string getNvsKeyByRcId(RcId id) const;
};

#endif
