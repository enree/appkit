#pragma once

#include <string>

namespace appkit
{

/**
 * Main application paths
 */
struct Paths
{
    std::string configs;
    std::string plugins;
    std::string cache;
    std::string translations;
    std::string logs;

    std::string configFile;
};

std::ostream& operator<<(std::ostream& out, const Paths& paths);

} // namespace appkit
