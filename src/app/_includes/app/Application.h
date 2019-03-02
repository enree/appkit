#pragma once

#include "Paths.h"

#include "AppManifest.h"

#include "config/ConfigParser.h"

#include <QCoreApplication>

#include <boost/noncopyable.hpp>

namespace appkit
{

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
};

} // namespace appkit
