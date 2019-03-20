/** @file
 * @brief Testing dummy type
 *
 * $Id: $
 */

#include "DummyObject.h"

#include <sstream>

namespace dummy
{

DummyObject::DummyObject(int k, const std::string& v) : m_key(k), m_value(v) {}

int DummyObject::key() const
{
    return m_key;
}

std::string DummyObject::value() const
{
    return m_value;
}

void DummyObject::setValue(const std::string& value)
{
    m_value = value;
}

std::string DummyObject::toString() const
{
    std::ostringstream stream;
    stream << m_key << " " << m_value;
    return stream.str();
}

DummyObject DummyObject::fromString(const std::string& str)
{
    std::istringstream stream(str);
    int key;
    std::string value;
    stream >> key >> value;
    return DummyObject(key, value);
}

std::ostream& operator<<(std::ostream& out, const DummyObject& obj)
{
    out << obj.toString();
    return out;
}

bool operator==(const DummyObject& left, const DummyObject& right)
{
    return left.key() == right.key() && left.value() == right.value();
}

std::vector<DummyObject> testData()
{
    std::vector<DummyObject> data;

    data.push_back(DummyObject(0, "zero"));
    data.push_back(DummyObject(1, "one"));
    data.push_back(DummyObject(2, "three"));

    return data;
}

} // namespace dummy
