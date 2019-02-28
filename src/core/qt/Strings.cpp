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

std::ostream& operator<<(std::ostream& output, const QString& str)
{
    output << strings::toUtf8(str);
    return output;
}

std::istream& operator>>(std::istream& input, QString& str)
{
    // We should read the whole input buffer
    std::istreambuf_iterator<char> eos;
    std::string string(std::istreambuf_iterator<char>(input), eos);

    str = strings::fromUtf8(string);
    return input;
}
