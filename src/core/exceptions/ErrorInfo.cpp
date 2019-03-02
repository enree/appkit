/** @file
 * @brief Commonly used error_info
 *
 * @ingroup
 *
 * $Id: $
 */

#include "exception/ErrorInfo.h"

#include <boost/format.hpp>

namespace exception
{

ForeignExceptionInfo toForeignExceptionInfo(const std::exception& ex)
{
    ForeignExceptionDetails details;
    details.type = typeid(ex).name();
    details.what = ex.what();
    return ForeignExceptionInfo(details);
}

std::ostream&
operator<<(std::ostream& stream, const ForeignExceptionDetails& details)
{
    stream << (boost::format("type: [%1$s], what: [%2$s]") % details.type
               % details.what)
                  .str();
    return stream;
}

} // namespace exception
