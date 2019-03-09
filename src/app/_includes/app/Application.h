#pragma once

#include "Paths.h"

#include "AppManifest.h"

#include "config/ConfigParser.h"
#include "log/Log.h"

#include "exception/ErrorInfo.h"

#include <QCoreApplication>

#include <boost/noncopyable.hpp>

namespace appkit
{

class InvalidLoggerConfiguration : public exception::Config
{
    using LoggerPath = boost::error_info<struct loggerPath_, std::string>;

public:
    explicit InvalidLoggerConfiguration(const std::string& loggerPath)
    {
        (*this) << LoggerPath(loggerPath);
    }
};

class Application : private boost::noncopyable
{
public:
    Application(
        int argc,
        char** argv,
        const appkit::AppManifest& manifest,
        QCoreApplication* app,
        appkit::Paths paths);

    virtual int run();

private:
    // Add specific application options
    virtual void addSpecificOptions(appkit::config::ConfigParser& configParser);

private:
    int m_argc;
    char** m_argv;
    const AppManifest& m_manifest;
    config::ConfigParser m_configParser;
    Paths m_paths;
    std::unique_ptr<logger::Log> m_logger;
};

} // namespace appkit
