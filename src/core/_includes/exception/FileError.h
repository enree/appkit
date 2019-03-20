/**
 * @file
 * @brief Input/output file exception
 *
 * $Id: $
 *
**/

#pragma once

#include "StreamError.h"

#include <boost/filesystem/path.hpp>

namespace exception
{

/**
 * This exception is trown in case of failed file input/output operations
**/
class FileError:
        public virtual StreamError
{
};

/**
 * Type for passing file info to exception.
 */
typedef boost::error_info<struct valueInfo_,
        boost::filesystem::path::string_type> FileInfo;

} // exception
