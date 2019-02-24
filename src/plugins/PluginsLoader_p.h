/**
 * @ingroup RIO_CORIOLIS
 * @file
 * @brief      Private implementation of PluginsLoader
 * @copyright  (C) 2015 PKB RIO Design Department
 *
 * $Id: $
 */

#pragma once

#include "PluginWrapper.h"

#include "coriolis/boost/foreach.h"

#include <QPluginLoader>

#ifndef Q_MOC_RUN   // Protect from BOOST_JOIN error
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#endif

#include <vector>

namespace appkit
{

namespace plugins
{

class PluginsLoader_p
        : public QObject
{
    Q_OBJECT
    friend class PluginsLoader;
signals:
    /**
     * The signal is emmited when loader trying to load @a plugin
     */
    void probing(const QString& plugin);
    /**
     * The signal is emmited when @a plugin has been loaded
     */
    void loaded(const QString&  plugin);
    /**
     * The signal is emmited when @a plugin has been skipped
     */
    void skipped(const QString&  plugin);
    /**
     * The signal is emmited when @a plugin failed to load
     */
    void failed(const QString& plugin, const QString& error);

public:
    typedef boost::shared_ptr<PluginWrapper> PluginWrapperPtr;
    template <typename I>
    std::vector<PluginWrapperPtr> take()
    {
        std::vector<PluginWrapperPtr> _plugins;

        auto it = m_plugins.begin();
        for (; it != m_plugins.end();)
        {
            if ((*it)->isInterfaceSupported<I>())
            {
                _plugins.push_back(*it);
                it = m_plugins.erase(it);
            }
            else
            {
                ++it;
            }
        }
        return _plugins;
    }

private:
    explicit PluginsLoader_p(QObject* parent = 0);

    size_t size() const;

    void load(bool serviceMode, const QString &subDir, const QString &baseDir);

    void add(const QString &name, QObject *object);

    template <typename I>
    std::vector<PluginWrapperPtr> plugins() const
    {
        std::vector<PluginWrapperPtr> plugins;
        BOOST_FOREACH(const Plugins::value_type& plugin, m_plugins)
        {
            if (plugin->isInterfaceSupported<I>())
            {
                plugins.push_back(plugin);
            }
        }
        return plugins;
    }

private:
    // Internal plugins storage
    typedef std::vector< boost::shared_ptr<PluginWrapper> > Plugins;
    Plugins m_plugins;
};

} // plugins

} // appkit
