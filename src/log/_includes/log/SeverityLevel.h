/** @file
 * @brief Defines severity level here
 *
 * @ingroup
 *
 *
 * $Id: $
 */

#pragma once

#include <iostream>

namespace appkit
{

namespace logger
{

enum SeverityLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

// The operator puts a human-friendly representation of the severity level to the stream
std::string toString(SeverityLevel level);
std::ostream& operator<< (std::ostream& stream, SeverityLevel level);

// The operator converts std::string to SeverityLevel
SeverityLevel toLevel(std::string value);
std::istream& operator>> (std::istream& stream, SeverityLevel& level);

} // logger

} // appkit
