#include "Paths.h"

#include "shakespear/Translate.h"

#include <iomanip>
#include <iostream>

namespace shakespear
{

namespace appkit
{

std::ostream& operator<<(std::ostream& out, const Paths& paths)
{
    out << SHAKESPEAR_TR("Config path: ") << paths.configs << std::endl
        << SHAKESPEAR_TR("Plugin path: ") << paths.plugins << std::endl
        << SHAKESPEAR_TR("Cache path: ") << paths.cache << std::endl
        << SHAKESPEAR_TR("Log path: ") << paths.logs << std::endl;
    return out;
}

} // namespace appkit

} // namespace shakespear
