/** @file
 * @brief ValueAttribute interface
 *
 * $Id: $
 */

#pragma once

#include <string>

namespace storage
{

/**
 * Value attribute interface.
 * Provides some unique identity key from a value, and converts it to id
 */
template <typename V, typename K, typename I>
class ValueAttribute
{
public:
    using ValueType = V;
    using KeyType = K;
    using IdType = I;

public:
    /**
     * Retrieve key from passed value
     */
    virtual KeyType key(const ValueType& value) const = 0;

    /**
     * Convert key to some id. ids should be equal
     * for equal keys and differ for different ones
     */
    virtual IdType id(const KeyType& key) const = 0;

    /**
     * Get id by passed @a value.
     * Actually _V is a ValueType. But when KeyType==ValueType we
     * have a compiler error because of above function, so we use
     * template function here.
     */
    template <typename _V>
    inline IdType id(const _V& value) const
    {
        return id(key(value));
    }

    virtual ~ValueAttribute() {}
};

} // namespace storage
