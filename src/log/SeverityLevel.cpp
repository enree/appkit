/** @file
 * @brief Defines severity level here
 *
 * @ingroup
 *
 * @copyright  (C) 2016 PKB RIO Design Department
 *
 * $Id: $
 */

#include "SeverityLevel.h"

#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/range/algorithm/find_if.hpp>

#include <typeinfo>
#include <utility>
#include <vector>

namespace rio
{

namespace logger
{

namespace
{

using Level = std::pair<SeverityLevel, std::string>;
using Levels = std::vector<Level>;

Levels levels()
{
    static Levels levels;
    if (levels.empty())
    {
        levels.push_back(std::make_pair(DEBUG, "DEBUG"));
        levels.push_back(std::make_pair(INFO, "INFO"));
        levels.push_back(std::make_pair(WARNING, "WARNING"));
        levels.push_back(std::make_pair(ERROR, "ERROR"));
        levels.push_back(std::make_pair(FATAL, "FATAL"));
    }
    return levels;
}

} // namespace

std::ostream &operator<<(std::ostream &stream, SeverityLevel level)
{
    stream << toString(level);
    return stream;
}

std::istream &operator>>(std::istream &stream, SeverityLevel &level)
{
    std::string value;
    stream >> value;
    level = toLevel(value);
    return stream;
}

std::string toString(SeverityLevel level)
{
    const Levels& lvls = levels();
    auto it = boost::find_if(lvls, boost::bind(&Level::first, _1) == level);
    if (it != lvls.end())
    {
        return it->second;
    }
    throw std::runtime_error("No conversion from SeverityLevel");
}

SeverityLevel toLevel(std::string value)
{
    boost::trim(value);
    const Levels& lvls = levels();
    auto it = boost::find_if(lvls, boost::bind(&Level::second, _1) == value);
    if (it != lvls.end())
    {
        return it->first;
    }
    throw std::runtime_error("No conversion to SeverityLevel");
}

} // namespace logger

} // namespace rio
