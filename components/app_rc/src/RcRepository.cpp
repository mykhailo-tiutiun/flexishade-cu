#include "rc/RcRepository.hpp"
#include <cstdint>

RcRepository::~RcRepository()
{
    reset();
}

void RcRepository::save(Rc rc)
{
    auto addr = rc.getAddr();

    rcs_.insert({addr, std::move(rc)});
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

std::optional<Rc> RcRepository::getByAddr(const std::uint64_t& id) const
{
    auto it = rcs_.find(id);

    if (it == rcs_.end()) {
        return {};
    }

    return it->second;
}
