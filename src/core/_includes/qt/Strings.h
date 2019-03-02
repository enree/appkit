/** @file
 * @brief string <-> QString converters
 *
 * $Id: $
 */

#pragma once

#include <QString>

#include <iostream>
#include <string>

std::ostream& operator<<(std::ostream& output, const QString& str);
std::istream& operator>>(std::istream& input, QString& str);

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

} // namespace strings
