/** @file
 * @brief Serializer interface
 *
 * $Id: $
 */

#pragma once

#include <string>

namespace serialization
{

/**
 * Serializer interface.
 * Should provide Type<->std::string conversion
 */
template <typename Type>
class Serializer
{
public:
    virtual std::string serialize(const Type& value) const = 0;

    /**
     * Deserialize value from string representation
     */
    virtual Type deserialize(const std::string& data) const = 0;

    virtual ~Serializer() {}
};

} // serialization
