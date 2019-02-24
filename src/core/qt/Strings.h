/** @file
 * @brief string <-> QString converters
 *
 * $Id: $
 */

#pragma once

#include <QString>

#include <string>

namespace strings
{
    /**
     * Convert utf8 std::string @a str to QString.
     */
    QString fromUtf8(const std::string& str);

    /**
     * Convert QString @a str to utf8-encoded std::string.
     */
    std::string toUtf8(const QString& str);

} // strings
