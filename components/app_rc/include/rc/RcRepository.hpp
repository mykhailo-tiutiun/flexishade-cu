#ifndef RC_REPOSITORY_HPP
#define RC_REPOSITORY_HPP

#include "rc/Rc.hpp"
#include <unordered_map>
#include <vector>

class RcRepository {
    public:
        ~RcRepository();

        std::optional<Rc> getById(RcId id) const;
        std::vector<Rc> getAll() const;

        void save(Rc rc);

        void reset();
    private:
        std::unordered_map<RcId, Rc> rcs_;
};

#endif
