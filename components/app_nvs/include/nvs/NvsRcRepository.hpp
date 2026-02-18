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

class NvsRcRepository : public RcRepository {
    public:
        NvsRcRepository(Nvs* nvs);

        std::expected<Rc, Error<RcRepositoryError>> getById(RcId id) const override;
        std::expected<std::vector<Rc>, Error<RcRepositoryError>> getAll() const override;

        std::expected<void, Error<RcRepositoryError>> save(Rc rc) override;
        std::expected<void, Error<RcRepositoryError>> reset() override;

        std::string_view getRepositroyId() const override
        {
            return "rc";
        }

    private:
        Nvs* nvs_;

        std::string getNvsIdByRcId(RcId id) const;
};

#endif
