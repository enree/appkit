#include "Application.h"

#include "UnixSignalHandler.h"

#include "ReturnCode.h"

#include "shakespear/Log.h"
#include "shakespear/Translate.h"

#include "coriolis/config/PTreeRW.h"
#include "coriolis/qt/StringUtils.h"
#include "coriolis/qt/TranslationInstaller.h"

#include <boost/algorithm/string/join.hpp>

namespace
{

void loadTranslations(const std::string& translationsPath)
{
    // Boost translations
    const std::vector<std::string> domains
        = rio::translate::installBoostTranslation(translationsPath);
    SHAKESPEAR_DEBUG << "Installed boost translations: "
                            + boost::algorithm::join(domains, ", ");

    // Qt translations
    const QStringList translations
        = QStringList() << rio::qt::installUserTranslations(
                               rio::strings::fromUtf8(translationsPath))
                        << rio::qt::installSystemTranslation("qt")
                        << rio::qt::installSystemTranslation("qt_help");
    SHAKESPEAR_DEBUG << "Installed qt translations: "
                            + rio::strings::toUtf8(translations.join(", "));
}

} // namespace

namespace shakespear
{

namespace appkit
{

using namespace rio::config;
using rio::config::ConfigParser;

Application::Application(
    int argc,
    char** argv,
    const rio::app::AppManifest& manifest,
    QCoreApplication* app,
    shakespear::appkit::Paths paths)
    : m_argc(argc)
    , m_argv(argv)
    , m_manifest(manifest)
    , m_paths(std::move(paths))
{
    loadTranslations(m_paths.translations);
    installSignalHandler(app);
}

int Application::run()
{
    addSpecificOptions(m_configParser);

    ConfigParser::ProgramOptions infoOptions
        = m_configParser.infoOptions(m_argc, m_argv);

    // First of all check if we need to show help
    if (infoOptions.count(ConfigParser::HELP) > 0)
    {
        m_configParser.printUsage();
        return 0;
    }

    // Second of all check if we need to show version
    if (infoOptions.count(ConfigParser::VERSION) > 0)
    {
        std::cout << m_manifest << std::endl << m_paths;
        return 0;
    }

    // Add tree
    m_configParser.addPtree(
        fromFile<format::INFO>(configFileName(
            m_argc, m_argv, m_paths.configs + "/" + m_paths.configFile)),
        SHAKESPEAR_TR("common config file"));

    // Add tree from command line, it will overwrite config values
    m_configParser.addPtree(
        rio::config::
            fromCommandLine(m_argc, m_argv, m_configParser.keys(), false),
        SHAKESPEAR_TR("command line"));

    try
    {
        m_configParser.readAll();
    }
    catch (const std::exception& ex)
    {
        SHAKESPEAR_FATAL("Config", "read") << ex.what();
        return shakespear::CONFIGURATION_ERROR;
    }

    return QCoreApplication::exec();
}

void Application::addSpecificOptions(ConfigParser& configParser)
{
    // Do nothing function
    Q_UNUSED(configParser);
}

} // namespace appkit

} // namespace shakespear
