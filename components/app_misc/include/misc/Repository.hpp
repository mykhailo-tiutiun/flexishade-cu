#ifndef REPOSITORY_HPP
#define REPOSITORY_HPP

enum class RepositoryError
{
    NotFound,
    Other
}

template<typename K, typename V>
class Repository
{
    public:
        virtual std::expected<V, Error<RepositoryError>> getById(K key) const = 0;
        virtual std::expected<std::vector<V>, Error<RepositoryError>> getAll() const = 0;

        virtual std::expected<void, Error<RepositoryError>> save(V value) const = 0;
        virtual std::expected<void, Error<RepositoryError>> reset() const = 0;
};

#endif
