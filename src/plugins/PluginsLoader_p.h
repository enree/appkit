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

#include <QPluginLoader>

#include <vector>

namespace appkit
{

namespace plugins
{

class PluginsLoader_p : public QObject
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
    void loaded(const QString& plugin);
    /**
     * The signal is emmited when @a plugin has been skipped
     */
    void skipped(const QString& plugin);
    /**
     * The signal is emmited when @a plugin failed to load
     */
    void failed(const QString& plugin, const QString& error);

public:
    using PluginWrapperPtr = std::shared_ptr<PluginWrapper>;
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

    void load(bool serviceMode, const QString& subDir, const QString& baseDir);

    void add(const QString& name, QObject* object);

    template <typename I>
    std::vector<PluginWrapperPtr> plugins() const
    {
        std::vector<PluginWrapperPtr> plugins;
        std::copy_if(
            m_plugins.begin(),
            m_plugins.end(),
            std::back_inserter(plugins),
            [](const std::shared_ptr<PluginWrapper>& plugin) {
                return plugin->isInterfaceSupported<I>();
            });

        return plugins;
    }

private:
    // Internal plugins storage
    using Plugins = std::vector<std::shared_ptr<PluginWrapper>>;
    Plugins m_plugins;
};

} // namespace plugins

} // namespace appkit
