#ifndef CLONABLE_HPP
#define CLONABLE_HPP

#include <memory>

template <typename T>
class Clonable
{
    public:
        virtual ~Clonable() = default;
        virtual std::unique_ptr<T> clone() const = 0;
};

#endif
