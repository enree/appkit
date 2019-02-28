/** @file
 * @brief Helper class for adding Params attribute to logger
 *
 * @ingroup
 *
 * @copyright  (C) 2016 PKB RIO Design Department
 *
 * $Id: $
 */

#include "LogParams.h"

#include <boost/algorithm/string/join.hpp>

namespace appkit
{

namespace logger
{

std::ostream& operator<<(std::ostream& stream, const LogParams& level)
{
    stream << boost::algorithm::join(level.m_params, ", ");
    return stream;
}

} // namespace logger

} // namespace appkit
