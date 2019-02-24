/** @file
 * @brief     Output to Boost.Log comma-separated values from std::map.
 *
 * @addtogroup RIO_MODULES_RIOLOG
 *
 * @copyright  (C) 2015-08-07 PKB RIO Design Department
 *
 * $Id: $
 */

#pragma once

#include "ContainerPrinter.h"

#include <map>


namespace std
{

template< typename Key, typename Value >
inline std::ostream&
operator<< (std::ostream& strm, const std::map<Key, Value>& container)
{
    return print(strm, container, "map");
}

} // std
