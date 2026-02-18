#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>
#include <utility>
#include <expected>

template <typename Kind>
class Error
{
    public:
        Error(Kind kind, std::string msg)
        : kind_(kind)
        , msg_(std::move(msg))
        {}

        template<typename OtherKind>
        Error(Kind kind, const Error<OtherKind>& other)
        : kind_(kind)
        , msg_(other.msg())
        {}

        Kind kind() const
        {
            return kind_;
        }

        const std::string& msg() const
        {
            return msg_;
        }

    private:
        Kind kind_;
        std::string msg_;
};

template<typename Kind>
std::unexpected<Error<Kind>> make_error(Kind kind, std::string msg) {
    return std::unexpected(Error<Kind>(kind, std::move(msg)));
}

template<typename Kind, typename OtherKind>
std::unexpected<Error<Kind>> make_error(Kind kind, const Error<OtherKind>& other) {
    return std::unexpected(Error<Kind>(kind, other));
}

#endif
