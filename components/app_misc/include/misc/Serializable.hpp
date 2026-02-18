#ifndef SERIALIZABLE_HPP
#define  SERIALIZABLE_HPP

#include "Error.hpp"

enum SerializationError {
    INVALID_BLOB_LEN,
    OTHER
};

class Serializable
{
    virtual std::vector<std::uint8_t> serialize() const;
    virtual std::expected<void, Error<SerializationError>> desirialize(std::vector<std::uint8_t> blob);
}

#endif
