/**
 * @file
 * @brief Invalid plugin exception
 *
 * @ingroup RIO_CORIOLIS
 *
 * @copyright  (C) 2015 PKB RIO Design Department
 *
 * $Id: $
 *
**/

#pragma once

#include "General.h"

namespace rio
{

namespace exception
{

/**
 * This exception is thrown when loading plugin failed unexpectedly
**/  
class InvalidPlugin:
    public virtual General
{
};

} // exception

} // rio

