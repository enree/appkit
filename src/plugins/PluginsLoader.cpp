/**
 * @ingroup RIO_CORIOLIS
 * @file
 * @brief      Plugins loader
 * @copyright  (C) 2015 PKB RIO Design Department
 *
 * $Id: $
 */

#include "PluginsLoader.h"

namespace appkit
{

namespace plugins
{

PluginsLoader::PluginsLoader(QObject* parent)
    : QObject(parent), m_impl(new PluginsLoader_p(this))
{
    connect(m_impl, &PluginsLoader_p::probing, this, &PluginsLoader::probing);
    connect(m_impl, &PluginsLoader_p::loaded, this, &PluginsLoader::loaded);
    connect(m_impl, &PluginsLoader_p::failed, this, &PluginsLoader::failed);
    connect(m_impl, &PluginsLoader_p::skipped, this, &PluginsLoader::skipped);
}

void PluginsLoader::load(
    bool serviceMode, const QString& subDir, const QString& baseDir)
{
    m_impl->load(serviceMode, subDir, baseDir);
}

size_t PluginsLoader::size() const
{
    return m_impl->size();
}

void PluginsLoader::add(const QString& name, QObject* object)
{
    m_impl->add(name, object);
}

PluginsLoader::iterator PluginsLoader::begin() const
{
    return iterator(m_impl->m_plugins.begin());
}

PluginsLoader::iterator PluginsLoader::end() const
{
    return iterator(m_impl->m_plugins.end());
}

} // namespace plugins

} // namespace appkit
