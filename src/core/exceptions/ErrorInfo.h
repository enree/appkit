/** @file
 * @brief Commonly used error_info
 *
 * @ingroup
 *
 * $Id: $
 */

#pragma once

#include <boost/exception/all.hpp>

namespace exception
{

/**
 * Type for passing extra details to exception. One can use it to pass
 * arbitrary string to exception
 */
typedef boost::error_info<struct exceptionInfo_, std::string> ExceptionInfo;

/**
 * Combine type of exception and it's description
 */
struct ForeignExceptionDetails
{
    std::string type; // Type of caught exception
    std::string what; // String returned by "what" method of caught exception
};

/**
 * This type can be used for passing info from caught exception to new one
 */
typedef boost::error_info<struct caughtException_, ForeignExceptionDetails> ForeignExceptionInfo;

/**
 * Convert exception to ForeignExceptionInfo
 */
ForeignExceptionInfo toForeignExceptionInfo(const std::exception& ex);

/**
 * Stream operator for ForeignExceptionDetails
 */
std::ostream& operator<<(std::ostream& stream, const ForeignExceptionDetails& details);

} // exception
