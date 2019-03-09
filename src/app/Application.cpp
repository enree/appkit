#include "app/Application.h"

#include "app/ReturnCode.h"
#include "app/UnixSignalHandler.h"

#include "appkit/Translate.h"
#include "log/Log.h"

#include "config/PTreeRW.h"
#include "qt/Strings.h"
#include "qt/TranslationInstaller.h"

#include <boost/algorithm/string/join.hpp>

namespace
{

void loadTranslations(const std::string& translationsPath)
{
    // Boost translations
    const std::vector<std::string> domains
        = appkit::translate::installBoostTranslation(translationsPath);
    LOG_DEBUG << "Installed boost translations: "
                     + boost::algorithm::join(domains, ", ");

    // Qt translations
    const QStringList translations = QStringList()
                                     << qt::installUserTranslations(
                                            strings::fromUtf8(translationsPath))
                                     << qt::installSystemTranslation("qt")
                                     << qt::installSystemTranslation("qt_help");
    LOG_DEBUG << "Installed qt translations: "
                     + strings::toUtf8(translations.join(", "));
}

} // namespace

namespace appkit
{

Application::Application(
    int argc,
    char** argv,
    const appkit::AppManifest& manifest,
    QCoreApplication* app,
    appkit::Paths paths)
    : m_argc(argc)
    , m_argv(argv)
    , m_manifest(manifest)
    , m_paths(std::move(paths))
{
    auto configLoggerPath
        = m_paths.configs + "/" + m_manifest.configBase() + ".log.ini";

    try
    {
        m_logger = std::make_unique<logger::Log>(configLoggerPath);
    }
    catch (const std::exception& ex)
    {
        BOOST_THROW_EXCEPTION(
            InvalidLoggerConfiguration(configLoggerPath)
            << exception::toForeignExceptionInfo(ex));
    }

    loadTranslations(m_paths.translations);
    installSignalHandler(app);
}

int Application::run()
{
    using config::ConfigParser;

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
        config::fromFile<config::format::INFO>(config::configFileName(
            m_argc, m_argv, m_paths.configs + "/" + m_paths.configFile)),
        APPKIT_TR("common config file"));

    // Add tree from command line, it will overwrite config values
    m_configParser.addPtree(
        config::fromCommandLine(m_argc, m_argv, m_configParser.keys(), false),
        APPKIT_TR("command line"));

    try
    {
        m_configParser.readAll();
    }
    catch (const std::exception& ex)
    {
        LOG_FATAL << ex.what();
        return appkit::CONFIGURATION_ERROR;
    }

    return QCoreApplication::exec();
}

void Application::addSpecificOptions(config::ConfigParser& configParser)
{
    // Do nothing function
    Q_UNUSED(configParser);
}

} // namespace appkit
