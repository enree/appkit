/**
 * @ingroup RIO_CORIOLIS
 * @file
 * @brief      Plugins loader
 * @copyright  (C) 2015 PKB RIO Design Department
 *
 * $Id: $
 */

#include "PluginsLoader.h"

#include "coriolis/qt/Signals.h"


namespace rio
{

namespace plugins
{

PluginsLoader::PluginsLoader(QObject *parent)
    : QObject(parent)
    , m_impl(new PluginsLoader_p(this))
{
    signal::connect_assert(m_impl, SIGNAL(probing(QString)), this, SIGNAL(probing(QString)));
    signal::connect_assert(m_impl, SIGNAL(loaded(QString)), this, SIGNAL(loaded(QString)));
    signal::connect_assert(m_impl, SIGNAL(failed(QString, QString)), this, SIGNAL(failed(QString, QString)));
    signal::connect_assert(m_impl, SIGNAL(skipped(QString)), this, SIGNAL(skipped(QString)));
}

void PluginsLoader::load(bool serviceMode, const QString &subDir, const QString &baseDir)
{
    m_impl->load(serviceMode, subDir, baseDir);
}

size_t PluginsLoader::size() const
{
    return m_impl->size();
}

void PluginsLoader::add(const QString &name, QObject *object)
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

} // namespace rio
