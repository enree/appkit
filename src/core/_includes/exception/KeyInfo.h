/** @file
 * @brief Info abou missing or duplicate keys
 *
 * $Id: $
 */

#pragma once

#include <boost/exception/all.hpp>
#include <boost/lexical_cast.hpp>

#include <vector>

namespace exception
{

/**
 * Type for passing any key to exception.
 */
typedef boost::error_info<struct keyInfo_, std::string> KeyInfo;

/**
 * Convert arbitrary key to string
 */
template <typename T>
KeyInfo toKeyInfo(T key)
{
    return KeyInfo(boost::lexical_cast<std::string>(key));
}

/**
 * Type for passing several keys to exception.
 */
typedef boost::error_info<struct keysInfo_, std::vector<std::string>> KeysInfo;

} // namespace exception
