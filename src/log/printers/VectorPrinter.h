/** @file
 * @brief      Output to Boost.Log comma-separated values from std::vector.
 *
 * @addtogroup RIO_MODULES_RIOLOG
 *
 * @copyright  (C) 2015-08-07 PKB RIO Design Department
 *
 * $Id: $
 */

#pragma once

#include "ContainerPrinter.h"

#include <vector>

namespace std
{

std::ostream&
operator<< (std::ostream& strm, const std::vector<int>& container)
{
    return print(strm, container, "vector");
}

} // std
