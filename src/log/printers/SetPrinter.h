/** @file
 * @brief     Output to Boost.Log comma-separated values from std::vector.
 *
 * @addtogroup RIO_MODULES_RIOLOG
 *
 * @copyright  (C) 2015-08-07 PKB RIO Design Department
 *
 * $Id: $
 */

#pragma once

#include "ContainerPrinter.h"

#include <set>


namespace std
{

template< typename Type >
inline std::ostream&
operator<< (std::ostream& strm, const std::set<Type>& container)
{
    return print(strm, container, "set");
}

} // std
