/**
 * @file
 * @brief      Plugins loader
 *
 * $Id: $
 */

#include "plugins/PluginsLoader.h"

#include "plugins/ServicePluginInterface.h"

#include "PluginException.h"

#include "exception/InvalidPath.h"
#include "qt/Strings.h"

#include <QDir>
#include <QLibrary>

#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/bind.hpp>

namespace appkit
{

namespace plugins
{

PluginsLoader::PluginsLoader(QObject* parent) : QObject(parent) {}

void PluginsLoader::load(
    bool serviceMode, const QString& subDir, const QString& baseDir)
{
    QDir pluginDir(baseDir);
    if (!(pluginDir.exists()) || !pluginDir.cd(subDir))
    {
        BOOST_THROW_EXCEPTION(
            exception::InvalidPath(strings::toUtf8(pluginDir.absolutePath())));
    }

    QStringList pluginList
        = pluginDir.entryList(QStringList() << "*", QDir::Files);

    for (const auto& pluginName: pluginList)
    {
        emit probing(pluginName);
        bool pluginNameHasBeenLoaded = boost::algorithm::any_of(
            m_plugins,
            boost::bind(&PluginWrapper::libraryName, _1) == pluginName);
        if (pluginNameHasBeenLoaded || !QLibrary::isLibrary(pluginName))
        {
            emit skipped(pluginName);
            continue;
        }

        try
        {
            auto wrapper
                = std::make_shared<PluginWrapper>(pluginName, pluginDir);
            if (serviceMode
                || !wrapper->isInterfaceSupported<ServicePluginInterface>())
            {
                bool pluginClassHasBeenLoaded = boost::algorithm::any_of(
                    m_plugins,
                    boost::bind(&PluginWrapper::className, _1)
                        == wrapper->className());

                // Skip loaded classes
                if (!pluginClassHasBeenLoaded)
                {
                    m_plugins.push_back(wrapper);
                    emit loaded(pluginName);
                }
                else
                {
                    emit skipped(pluginName);
                }
            }
            else
            {
                emit skipped(pluginName);
            }
        }
        catch (const plugins::PluginException& ex)
        {
            auto info = boost::get_error_info<plugins::PluginError>(ex);
            emit failed(
                pluginName, info != nullptr ? *info : tr("Unknown error"));
        }
    }
}

size_t PluginsLoader::size() const
{
    return m_plugins.size();
}

void PluginsLoader::add(const QString& name, QObject* object)
{
    bool pluginClassHasBeenLoaded = boost::algorithm::
        any_of(m_plugins, boost::bind(&PluginWrapper::className, _1) == name);

    if (!pluginClassHasBeenLoaded)
    {
        m_plugins.push_back(std::make_shared<PluginWrapper>(name, object));
    }
}

PluginsLoader::iterator PluginsLoader::begin() const
{
    return iterator(m_plugins.begin());
}

PluginsLoader::iterator PluginsLoader::end() const
{
    return iterator(m_plugins.end());
}

} // namespace plugins

} // namespace appkit
