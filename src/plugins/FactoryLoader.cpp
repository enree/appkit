/** @file
 * @brief     Load plugins for factory
 *
 * @ingroup   RIO_NEWTON
 *
 * @copyright (C) 2017 PKB RIO Design Department
 *
 * $Id: $
 */

#include "FactoryLoader.h"

#include "coriolis/Translate.h"

#include "coriolis/exception/InvalidPath.h"
#include "coriolis/qt/Signals.h"
#include "coriolis/qt/StringUtils.h"

namespace rio
{

namespace plugins
{

void FactoryLoader::logPluginProbe(const QString& pluginName)
{
    m_logger(CORIOLIS_TR("Loading {1}", strings::toUtf8(pluginName)));
}

void FactoryLoader::logPluginFail(
    const QString& pluginName, const QString& error)
{
    m_logger(CORIOLIS_TR(
        "Failed to load {1}: {2}",
        strings::toUtf8(pluginName),
        strings::toUtf8(error)));
}

void FactoryLoader::logPluginSkip(const QString& pluginName)
{
    m_logger(CORIOLIS_TR("Skipped {1}", strings::toUtf8(pluginName)));
}

void FactoryLoader::logPluginSuccess(const QString& pluginName)
{
    m_logger(CORIOLIS_TR("Loaded {1}", strings::toUtf8(pluginName)));
}

void FactoryLoader::loadPlugins(
    PluginsLoader& pluginsLoader,
    const std::string& path,
    const std::string& subdir)
{
    m_logger(CORIOLIS_TR("Loading plugins from {1}/{2}", path, subdir));

    signal::connect_assert(
        &pluginsLoader,
        SIGNAL(probing(QString)), this, SLOT(logPluginProbe(QString)));

    signal::connect_assert(
        &pluginsLoader,
        SIGNAL(skipped(QString)), this, SLOT(logPluginSkip(QString)));

    signal::connect_assert(
        &pluginsLoader,
        SIGNAL(loaded(QString)), this, SLOT(logPluginSuccess(QString)));

    signal::connect_assert(
        &pluginsLoader,
        SIGNAL(failed(QString, QString)),
        this,
        SLOT(logPluginFail(QString, QString)));

    pluginsLoader
        .load(false, strings::fromUtf8(subdir), strings::fromUtf8(path));
}

} // namespace plugins

} // namespace rio
