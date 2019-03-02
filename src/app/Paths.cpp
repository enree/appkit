#include "Paths.h"

#include "appkit/Translate.h"

#include <iomanip>
#include <iostream>

namespace appkit
{

std::ostream& operator<<(std::ostream& out, const Paths& paths)
{
    out << APPKIT_TR("Config path: ") << paths.configs << std::endl
        << APPKIT_TR("Plugin path: ") << paths.plugins << std::endl
        << APPKIT_TR("Cache path: ") << paths.cache << std::endl
        << APPKIT_TR("Log path: ") << paths.logs << std::endl;
    return out;
}

} // namespace appkit
