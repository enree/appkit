#pragma once

#include "SeverityLevel.h"

#ifndef Q_MOC_RUN // Protect from BOOST_JOIN error

#include "LogParams.h"

#include <boost/log/common.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/filter_parser.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/utility/setup/from_settings.hpp>

#endif

#define DEFINE_LOGGER(LoggerName)                                              \
    BOOST_LOG_GLOBAL_LOGGER_INIT(                                              \
        LoggerName, boost::log::sources::severity_logger_mt)                   \
    {                                                                          \
        boost::log::register_simple_filter_factory<                            \
            appkit::logger::SeverityLevel,                                     \
            char>("Severity");                                                 \
        boost::log::sources::severity_logger_mt<appkit::logger::SeverityLevel> \
            lg;                                                                \
        return lg;                                                             \
    }

#define DECLARE_LOGGER(LoggerName)                                             \
    BOOST_LOG_GLOBAL_LOGGER(                                                   \
        LoggerName,                                                            \
        boost::log::sources::severity_logger_mt<                               \
            appkit::logger::SeverityLevel>)

namespace appkit
{

namespace logger
{

class Log : public boost::noncopyable
{
public:
    explicit Log(const std::string& configFilename);
    explicit Log(const boost::log::settings& settings);

    ~Log();

private:
    void setupAttributes();
};

} // namespace logger

} // namespace appkit

DECLARE_LOGGER(LOG)

// Define logger with location
#define LOG_WITH_LOCATION(LOGGER, LEVEL)                                       \
    BOOST_LOG_SEV(LOGGER, LEVEL)                                               \
        << boost::log::add_value("Line", __LINE__)                             \
        << boost::log::add_value("File", __FILE__)                             \
        << boost::log::add_value("Function", __FUNCTION__)

#define PARAMS(PARAMS)                                                         \
    boost::log::add_value("Params", appkit::logger::LogParams()(PARAMS))

#define LOG_DEBUG LOG_WITH_LOCATION(LOG::get(), appkit::logger::DEBUG)
#define LOG_INFO LOG_WITH_LOCATION(LOG::get(), appkit::logger::INFO)
#define LOG_WARNING LOG_WITH_LOCATION(LOG::get(), appkit::logger::WARNING)
#define LOG_ERROR LOG_WITH_LOCATION(LOG::get(), appkit::logger::ERROR)
#define LOG_FATAL LOG_WITH_LOCATION(LOG::get(), appkit::logger::FATAL)
