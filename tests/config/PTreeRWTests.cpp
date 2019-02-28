/** @file
 * @brief Test suit for testing ptree rw methods
 *
 * @ingroup
 *
 * @copyright  (C) 2017 PKB RIO Design Department
 *
 * $Id: $
 */

#include "config/PTreeRW.h"

#include "qt/Strings.h"

#include "gqtest"

#include <QString>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>

namespace appkit
{

const char ASCII[] = "ascii";
const char UTF8[] = "utf8";
const char ARRAY[] = "array";
const QString ARRAY_VALUE("array_value(%1)");
const QString ASCII_VALUE("ascii_value");
const QString UTF8_VALUE("Киррилические буквы");
const int arrayValues(5);

template <typename T>
class ConverterTest : public ::testing::Test
{
public:
    ConverterTest()
    {
        m_expected.add(ASCII, strings::toUtf8(ASCII_VALUE));
        m_expected.add(UTF8, strings::toUtf8(UTF8_VALUE));

        boost::property_tree::ptree nodes;
        for (int i = 0; i < arrayValues; ++i)
        {
            boost::property_tree::ptree node;
            node.put("", strings::toUtf8(ARRAY_VALUE.arg(i)));
            nodes.push_back(std::make_pair("", node));
        }
        m_expected.add_child(ARRAY, nodes);
    }

    boost::property_tree::ptree converted() const
    {
        std::string str = config::toString<T>(m_expected);
        std::cout << str;
        return config::fromString<T>(str);
    }

protected:
    boost::property_tree::ptree m_expected;
};

TYPED_TEST_CASE_P(ConverterTest);

TYPED_TEST_P(ConverterTest, RwTest)
{
    boost::property_tree::ptree actual = this->converted();

    EXPECT_EQ(ASCII_VALUE, strings::fromUtf8(actual.get<std::string>(ASCII)));
    EXPECT_EQ(UTF8_VALUE, strings::fromUtf8(actual.get<std::string>(UTF8)));

    int i(0);
    BOOST_FOREACH (
        boost::property_tree::ptree::value_type& v, actual.get_child("array."))
    {
        EXPECT_EQ(ARRAY_VALUE.arg(i++), strings::fromUtf8(v.second.data()));
    }
}

REGISTER_TYPED_TEST_CASE_P(ConverterTest, RwTest);

// format::XML save ptree array as empty node that leads to invalid xml
// format::INI can't add duplicate keys (so no array is available in INI)
// format::JSON has a bug with Unicode encoding/decoding in boost before 1.60
typedef ::testing::Types<
    config::format::INFO /*, format::XML, format::JSON, format::INI*/>
    Types;
INSTANTIATE_TYPED_TEST_CASE_P(Rw, ConverterTest, Types);

} // namespace appkit
