#include "nvs/NvsRcRepository.hpp"
#include "rc/Rc.hpp"
#include "misc/Error.hpp"
#include <string>
#include <expected>
#include <string_view>
#include "esp_log.h"

NvsRcRepository::NvsRcRepository(Nvs* nvs)
: nvs_(nvs)
{}

std::expected<Rc, Error<RcRepositoryError>> NvsRcRepository::getById(RcId id) const
{
    auto nvs_id = getNvsKeyByRcId(id);

    std::vector<std::uint8_t> buf(Rc::serializationSize());

    auto data_res = nvs_->load(nvs_id, buf);
    if (!data_res) {
        auto err = data_res.error();
        if (err.kind() == NvsError::NotFound) {
            return make_error(RcRepositoryError::NotFound, err);
        } else {
            return make_error(RcRepositoryError::Other, err);
        }
    }

    auto rc_res = Rc::desirialize(buf);
    if (!rc_res) {
        return make_error(RcRepositoryError::Other, rc_res.error());
    }

    return *rc_res;
}

std::expected<std::vector<Rc>, Error<RcRepositoryError>> NvsRcRepository::getAll() const
{
    return make_error(RcRepositoryError::Other, "not implemented");
}

std::expected<void, Error<RcRepositoryError>> NvsRcRepository::save(Rc rc) const
{
    auto nvs_id = getNvsKeyByRcId(rc.getId());

    auto data = rc.serialize();

    auto res = nvs_->save(nvs_id, data);

    if (!res) {
        return make_error(RcRepositoryError::Other, res.error());
    }

    return {};
}

std::expected<void, Error<RcRepositoryError>> NvsRcRepository::reset() const
{
    return make_error(RcRepositoryError::Other, "not implemented");
}

std::string NvsRcRepository::getNvsKeyByRcId(RcId id) const
{
    auto rc_id_str = id.asAscii85();

    return std::format("{}{}", NVS_KEY_PREFIX, rc_id_str);
}
