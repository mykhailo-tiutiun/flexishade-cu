#ifndef REPOSITORY_CACHE_HPP
#define REPOSITORY_CACHE_HPP

#include "misc/Error.hpp"
#include "misc/Repository.hpp"
#include <functional>
#include <unordered_map>
template<
    class K, class V,
    class Hash = std::hash<K>,
    class Eq = std::equal_to<K>
>
class RepositoryCache : public Repository<K, V>
{
    public:
    private:
        Repository<K, V>* inner_;
        mutable std::unordered_map<K, V> cache_;

        std::expected<void, Error<RepositoryError>> loadAll()
        {
            auto vals_res = inner_->getAll();
            if (!vals_res) {
                return std::unexpected(vals_res.error());
            }
            return {};
        }
};

#endif
