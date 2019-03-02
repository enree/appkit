/** @file
 * @brief Helper class for adding Params attribute to logger
 *
 * @ingroup
 *
 *
 * $Id: $
 */

#pragma once

#include <boost/lexical_cast.hpp>

#include <vector>
#include <string>

namespace appkit
{

namespace logger
{

/**
 * This class helps add Params to logger
 */
class LogParams
{
    friend std::ostream& operator<< (std::ostream& stream, const LogParams& level);

public:
    /**
     * Add single parameter to params
     */
    template <typename T>
    LogParams& operator()(const T& value)
    {
        m_params.push_back(boost::lexical_cast<std::string>(value));
        return *this;
    }

    /**
     * Add single named parameter to params
     */
    template <typename T>
    LogParams& operator()(const std::string& name, const T& value)
    {
        m_params.push_back(name + "=" + boost::lexical_cast<std::string>(value));
        return *this;
    }

private:
    std::vector<std::string> m_params;
};

// To stream operator. Required for formatter
std::ostream& operator<< (std::ostream& stream, const LogParams& level);

} // logger

} // appkit
