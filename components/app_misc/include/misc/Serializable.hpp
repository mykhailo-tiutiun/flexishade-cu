#ifndef SERIALIZABLE_HPP
#define SERIALIZABLE_HPP

#include "Error.hpp"
#include <cstddef>
#include <cstdint>
#include <vector>
#include <expected>

enum class SerializationError {
    InvalidDataLength,
    Other
};

class Serializable
{
    public:
        virtual std::vector<std::uint8_t> serialize() const = 0 ;
        virtual std::expected<void, Error<SerializationError>> desirialize(const std::vector<std::uint8_t>& data) = 0;
        virtual std::size_t serializationSize() const = 0;
};

#endif
