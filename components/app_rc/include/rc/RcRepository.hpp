#ifndef RC_REPOSITORY_HPP
#define RC_REPOSITORY_HPP

#include "rc/RelayRc.hpp"
#include <cstdint>
#include <map>
#include <vector>

class RcRepository {
    public:
        ~RcRepository();

        std::optional<RelayRc> getByAddr(const std::uint64_t& id) const;
        std::vector<RelayRc> getAll() const;

        void save(RelayRc rc);

        void reset();
    private:
        std::map<std::uint64_t, RelayRc> rcs_;
};

#endif
