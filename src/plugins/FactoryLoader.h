/** @file
 * @brief     Load plugins for factory
 *
 * @ingroup   RIO_NEWTON
 *
 * @copyright (C) 2017 PKB RIO Design Department
 *
 * $Id: $
 */

#pragma once

#include "plugins/PluginsHolder.h"

#include <QObject>

#include <functional>

namespace appkit
{

namespace plugins
{

/**
 * Wrapper for loading specific plugins from specific folder.
 * Also provides notification capability (for splash screen)
 */
class FactoryLoader : public QObject
{
    Q_OBJECT
public:
    explicit FactoryLoader(
        const std::function<void(const QString&)>& logger,
        QObject* parent = nullptr);

    /**
     * Do actual plugins loading
     */
    template <typename PluginInterface>
    std::unique_ptr<plugins::PluginsHolder<PluginInterface>>
    load(const QString& path, const QString& subdir)
    {
        using namespace plugins;
        PluginsLoader pluginsLoader;
        loadPlugins(pluginsLoader, path, subdir);

        return std::make_unique<PluginsHolder<PluginInterface>>(pluginsLoader);
    }

private:
    void loadPlugins(
        PluginsLoader& pluginsLoader,
        const QString& path,
        const QString& subdir);

private:
    std::function<void(const QString&)> m_logger;
};

} // namespace plugins

} // namespace appkit
