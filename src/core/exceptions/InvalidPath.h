/** @file
 * @brief
 *
 * @ingroup
 *
 * @copyright  (C) 2016 PKB RIO Design Department
 *
 * $Id: $
 */

#pragma once

#include "General.h"

namespace exception
{

/**
 * Type for passing path to exception.
 */
typedef boost::error_info<struct pathInfo_, std::string> PathInfo;

/**
 * This exception is thrown when malformed path encountered
**/
class InvalidPath:
    public virtual General
{
public:
    InvalidPath(const std::string& path)
    {
        (*this) << PathInfo(path);
    }
};

} // exception
