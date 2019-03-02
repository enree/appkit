/** @file
 * @brief
 *
 * $Id: $
 */

#pragma once

#include "exception/General.h"

#include <QString>

namespace appkit
{

namespace plugins
{

/**
 * This exception is thrown when double insertion of unique type is performed
 **/
class PluginException : public virtual exception::General
{
};

/**
 * Type for passing file info to exception.
 */
typedef boost::error_info<struct valueInfo_, QString> PluginError;

} // namespace plugins

} // namespace appkit
