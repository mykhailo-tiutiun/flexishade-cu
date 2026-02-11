#ifndef RC_REPOSITORY_HPP
#define RC_REPOSITORY_HPP

#include "rc/Rc.hpp"
#include <cstdint>
#include <map>
#include <vector>

class RcRepository {
    public:
        ~RcRepository();

        std::optional<Rc> getByAddr(const std::uint64_t& id) const;
        std::vector<Rc> getAll() const;

        void save(Rc rc);

        void reset();
    private:
        std::map<std::uint64_t, Rc> rcs_;
};

#endif
