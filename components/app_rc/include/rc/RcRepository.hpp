#ifndef RC_REPOSITORY_HPP
#define RC_REPOSITORY_HPP

#include "misc/Error.hpp"
#include "rc/Rc.hpp"
#include <vector>

enum class RcRepositoryError
{
    NotFound,
    Other
};

class RcRepository
{
    public:
        virtual std::expected<Rc, Error<RcRepositoryError>> getById(RcId id) const = 0;
        virtual std::expected<std::vector<Rc>, Error<RcRepositoryError>> getAll() const = 0;

        virtual std::expected<void, Error<RcRepositoryError>> save(Rc rc) const = 0;
        virtual std::expected<void, Error<RcRepositoryError>> reset() const = 0;
};

#endif
