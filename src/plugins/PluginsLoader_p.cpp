/**
 * @ingroup RIO_CORIOLIS
 * @file
 * @brief      Private implementation of PluginsLoader
 * @copyright  (C) 2015 PKB RIO Design Department
 *
 * $Id: $
 */

#include "PluginsLoader_p.h"

#include "PluginException.h"
#include "ServicePluginInterface.h"

#include "coriolis/exception/InvalidPath.h"
#include "coriolis/qt/StringUtils.h"

#include <QDir>

#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>

namespace rio
{

namespace plugins
{

PluginsLoader_p::PluginsLoader_p(QObject* parent)
    : QObject(parent)
{
}

size_t PluginsLoader_p::size() const
{
    return m_plugins.size();
}

void PluginsLoader_p::load(bool serviceMode, const QString& subDir, const QString& baseDir)
{
    QDir pluginDir(baseDir);
    if (!(pluginDir.exists()) || !pluginDir.cd(subDir))
    {
        BOOST_THROW_EXCEPTION(exception::InvalidPath(strings::toUtf8(pluginDir.absolutePath())));
    }

    QStringList pluginList = pluginDir.entryList(QStringList() << "*", QDir::Files);

    BOOST_FOREACH (const QString& pluginName, pluginList)
    {
        emit probing(pluginName);
        bool pluginNameHasBeenLoaded = boost::algorithm::any_of(
            m_plugins, boost::bind(&PluginWrapper::libraryName, _1) == pluginName);
        if (pluginNameHasBeenLoaded || !QLibrary::isLibrary(pluginName))
        {
            emit skipped(pluginName);
            continue;
        }

        try
        {
            auto wrapper = boost::make_shared<PluginWrapper>(pluginName, pluginDir);
            if (serviceMode || !wrapper->isInterfaceSupported<ServicePluginInterface>())
            {
                bool pluginClassHasBeenLoaded = boost::algorithm::any_of(
                    m_plugins, boost::bind(&PluginWrapper::className, _1) == wrapper->className());

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
            emit failed(pluginName, info != nullptr ? *info: tr("Unknown error"));
        }
    }
}

void PluginsLoader_p::add(const QString &name, QObject *object)
{
    bool pluginClassHasBeenLoaded = boost::algorithm::any_of(
        m_plugins, boost::bind(&PluginWrapper::className, _1) == name);

    if (!pluginClassHasBeenLoaded)
    {
        m_plugins.push_back(boost::make_shared<PluginWrapper>(name, object));
    }
}

} // namespace plugins

} // namespace rio
