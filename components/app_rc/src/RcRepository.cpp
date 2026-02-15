#include "rc/RcRepository.hpp"

RcRepository::~RcRepository()
{
    reset();
}

void RcRepository::save(Rc rc)
{
    auto id = rc.getId();

    rcs_.insert_or_assign(id, std::move(rc));
}

void RcRepository::reset()
{
    rcs_.clear();
}

std::vector<Rc> RcRepository::getAll() const
{
    std::vector<Rc> vals;
    vals.reserve(rcs_.size());

    for (auto& [key, val] : rcs_) {
        vals.push_back(val);
    }

    return vals;
}

std::optional<Rc> RcRepository::getById(RcId id) const
{
    auto it = rcs_.find(id);

    if (it == rcs_.end()) {
        return {};
    }

    return it->second;
}
