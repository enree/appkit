/** @file
 * @brief Testing dummy type
 *
 * @ingroup
 * $Id: $
 */

#pragma once

#include <iostream>
#include <memory>
#include <vector>

namespace dummy
{

/**
 * Simple dummy object for testing storages
 */
class DummyObject
{
public:
    /**
     * Create dummy object
     */
    DummyObject(int k, const std::string& v);
    /**
     * Get object key
     */
    int key() const;
    /**
     * Get object value
     */
    std::string value() const;
    /**
     * Set object value
     */
    void setValue(const std::string& value);

    /**
     * Convert DummyObject to string
     */
    std::string toString() const;
    /**
     * Create DummyObject from string
     */
    static DummyObject fromString(const std::string& str);

private:
    int m_key;
    std::string m_value;
};

std::ostream& operator<<(std::ostream& out, const DummyObject& obj);
bool operator==(const DummyObject& left, const DummyObject& right);

using DummyPtr = std::shared_ptr<DummyObject>;
using ConstDummyPtr = std::shared_ptr<const DummyObject>;

/**
 * Create simple test data
 */
std::vector<DummyObject> testData();

} // namespace dummy
