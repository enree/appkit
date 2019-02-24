/** @file
 * @brief
 *
 * @ingroup
 *
 * @copyright  (C) 2017 PKB RIO Design Department
 *
 * $Id: $
 */

#pragma once

#include "coriolis/exception/General.h"

#include <QString>

namespace appkit
{

namespace plugins
{

/**
 * This exception is thrown when double insertion of unique type is performed
**/
class PluginException:
        public virtual exception::General
{
};

/**
 * Type for passing file info to exception.
 */
typedef boost::error_info<struct valueInfo_, QString> PluginError;

} // exception

} // appkit
