/** @file
 * @brief Useful functions
 *
 * $Id: $
 */

#include "Strings.h"

namespace strings
{

QString fromUtf8(const std::string& str)
{
    return QString::fromUtf8(str.c_str());
}

std::string toUtf8(const QString& str)
{
    QByteArray utf8 = str.toUtf8();
    return std::string(utf8.begin(), utf8.end());
}

} // namespace strings
