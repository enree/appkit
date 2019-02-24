#pragma once

#include "SeverityLevel.h"

#ifndef Q_MOC_RUN   // Protect from BOOST_JOIN error

#include "LogParams.h"

#include <boost/log/common.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/utility/setup/filter_parser.hpp>
#include <boost/log/utility/setup/from_settings.hpp>

#endif


// Define mutable constant to use in attribute (use in filter)
#define VALUE_ATTRIBUTE(NAME) \
    inline boost::log::attributes::mutable_constant<std::string>& NAME##Attr()\
    {\
        static boost::log::attributes::mutable_constant<std::string> attr(""); \
        return attr;\
    }

VALUE_ATTRIBUTE(object)
VALUE_ATTRIBUTE(action)

#define DEFINE_LOGGER(LoggerName) \
    BOOST_LOG_GLOBAL_LOGGER_INIT(LoggerName, boost::log::sources::severity_logger_mt) \
    { \
        boost::log::register_simple_filter_factory<appkit::logger::SeverityLevel, char>("Severity");\
        boost::log::sources::severity_logger_mt<appkit::logger::SeverityLevel> lg; \
        lg.add_attribute("Scope", boost::log::attributes::constant<std::string>(#LoggerName)); \
        lg.add_attribute("Object", objectAttr()); \
        lg.add_attribute("Action", actionAttr()); \
        return lg; \
    }

#define DECLARE_LOGGER(LoggerName) \
    BOOST_LOG_GLOBAL_LOGGER(LoggerName, boost::log::sources::severity_logger_mt<appkit::logger::SeverityLevel>)


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

} // log

} // appkit

