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

#include "coriolis/plugins/PluginsHolder.h"

#include <QObject>

#include <boost/function.hpp>
#include <boost/make_shared.hpp>

namespace appkit
{

namespace plugins
{

/**
 * Wrapper for loading specific plugins from specific folder.
 * Also provides notification capability (for splash screen)
 */
class FactoryLoader
    : public QObject
{
    Q_OBJECT
public:
    explicit FactoryLoader(
        const boost::function<void(const std::string&)>& logger,
        QObject* parent = NULL);

    /**
     * Do actual plugins loading
     */
    template <typename PluginInterface>
    boost::shared_ptr< plugins::PluginsHolder<PluginInterface> >
    load(const std::string& path, const std::string& subdir)
    {
        using namespace plugins;
        PluginsLoader pluginsLoader;
        loadPlugins(pluginsLoader, path, subdir);

        return boost::make_shared< PluginsHolder<PluginInterface> >
                (pluginsLoader);
    }

private slots:
    void logPluginProbe(const QString& pluginName);
    void logPluginFail(const QString& pluginName, const QString& error);
    void logPluginSkip(const QString& pluginName);
    void logPluginSuccess(const QString& pluginName);

private:
    void loadPlugins(
        PluginsLoader& pluginsLoader,
        const std::string& path,
        const std::string& subdir);

private:
    boost::function<void(const std::string&)> m_logger;
};

} // plugins

} // appkit
