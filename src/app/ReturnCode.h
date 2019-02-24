/** @file
 * @brief Application return codes
 *
 * @ingroup
 *
 * @copyright  (C) 2016 PKB RIO Design Department
 *
 * $Id: $
 */

#pragma once

namespace shakespear
{

enum ReturnCode
{
    SUCCESS = 0,
    RUN_TIME_ERROR,
    LOGGER_ERROR,
    INITIALIZATION_ERROR,
    CONFIGURATION_ERROR,
    DUPLICATE_RUN_ERROR
};

} // namespace shakespear
