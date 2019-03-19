/** @file
 * @brief
 *
 * $Id: $
 */

#pragma once

#include "coriolis/config/PTreeRW.h"
#include "coriolis/serialization/Serializer.h"

namespace serialization
{

/**
 * Empty class for template parameter binding
 */
class Void {};

/**
 * Property tree converters
 */
template <typename ValueType>
void toPropertyTree(
        const ValueType& value,
        boost::property_tree::ptree &tree);

template <typename ValueType, typename ValueFactory>
ValueType fromPropertyTree(
        const boost::property_tree::ptree &tree,
        ValueFactory factory);

template <typename ValueType>
ValueType fromPropertyTree(const boost::property_tree::ptree &tree);

/**
 * Helper class for calling proper fromPropertyTree function
 * This implementation call fromPropertyTree with ValueFactory argument
 */
template <typename ValueType, typename ValueFactory>
struct FromPropertyTreeHelper
{
    ValueType operator()(
            const boost::property_tree::ptree& tree,
            ValueFactory factory)
    {
        return fromPropertyTree<ValueType, ValueFactory>(tree, factory);
    }
};

/**
 * Helper class for calling proper fromPropertyTree function
 * This implementation call fromPropertyTree without ValueFactory argument,
 * omitting template Empty parameter.
 */
template <typename ValueType>
struct FromPropertyTreeHelper<ValueType, Void>
{
    ValueType operator()(
            const boost::property_tree::ptree& tree,
            Void voidValue)
    {
        (void) voidValue;
        return fromPropertyTree<ValueType>(tree);
    }
};

/**
 * Generic ptree serializer
 */
template <typename Type, typename Format, typename ValueFactory = Void>
class PtreeSerializer : public Serializer<Type>
{
public:
    PtreeSerializer(ValueFactory valueFactory = ValueFactory())
        : m_valueFactory(valueFactory)
    {
    }

    /**
     * Serialize value to string representation
     */
    virtual std::string serialize(const Type& value) const
    {
        boost::property_tree::ptree tree;
        toPropertyTree<Type>(value, tree);

        std::ostringstream stream;
        config::Writer<std::ostream&, Format>::write(tree, stream);
        return stream.str();
    }

    /**
     * Deserialize value from string representation
     */
    virtual Type deserialize(const std::string& data) const
    {
        std::istringstream stream(data);
        boost::property_tree::ptree tree
                = config::Reader<std::istream&, Format>::read(stream);

        return FromPropertyTreeHelper<Type, ValueFactory>()(tree, m_valueFactory);
    }

private:
    ValueFactory m_valueFactory;
};

} // serialization
