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

namespace appkit
{

namespace plugins
{

void FactoryLoader::loadPlugins(
    PluginsLoader& pluginsLoader, const QString& path, const QString& subdir)
{
    m_logger(tr("Loading plugins from {1}/{2}").arg(path, subdir));

    connect(
        &pluginsLoader,
        &PluginsLoader::probing,
        this,
        [this](const QString& pluginName) {
            m_logger(tr("Loading %1").arg(pluginName));
        });

    connect(
        &pluginsLoader,
        &PluginsLoader::skipped,
        this,
        [this](const QString& pluginName) {
            m_logger(tr("Skipped %1").arg(pluginName));
        });

    connect(
        &pluginsLoader,
        &PluginsLoader::loaded,
        this,
        [this](const QString& pluginName) {
            m_logger(tr("Loaded %1").arg(pluginName));
        });

    connect(
        &pluginsLoader,
        &PluginsLoader::failed,
        this,
        [this](const QString& pluginName, const QString& error) {
            m_logger(tr("Failed to load %1: %2").arg(pluginName, error));
        });

    pluginsLoader.load(false, subdir, path);
}

} // namespace plugins

} // namespace appkit
