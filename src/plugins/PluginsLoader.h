/**
 * @ingroup RIO_CORIOLIS
 * @file
 * @brief      Plugins loader
 * @copyright  (C) 2015 PKB RIO Design Department
 *
 * $Id: $
 */

#pragma once

#include "PluginsLoader_p.h"

#include "coriolis/utils/Range.h"

#include <QObject>

#ifndef Q_MOC_RUN   // Protect from BOOST_JOIN error
#include <boost/iterator/iterator_facade.hpp>
#endif

#include <vector>

namespace appkit
{

namespace plugins
{

/**
 * Plugins iterator type (const)
 */
class PluginsLoaderIterator :
        public boost::iterator_facade
        <
            PluginsLoaderIterator
            , boost::shared_ptr<PluginWrapper>  const
            , boost::forward_traversal_tag
        >
{
public:
    typedef boost::shared_ptr<PluginWrapper> value_type;

    explicit PluginsLoaderIterator(const std::vector<value_type>::const_iterator& it)
        : m_iterator(it)
    {
    }

private:
    friend class boost::iterator_core_access;

    const value_type& dereference() const
    {
        return *m_iterator;
    }

    void increment()
    {
        ++m_iterator;
    }

    bool equal(const PluginsLoaderIterator& other) const
    {
        return m_iterator == other.m_iterator;
    }

private:
    std::vector<value_type>::const_iterator m_iterator;
};

/**
 * Class for loading plugins. Sending signals about loading progress
 */
class PluginsLoader
    : public QObject
{
    Q_OBJECT

    template <class I> friend class PluginsHolder;
public:
    typedef PluginsLoaderIterator iterator;    // Type of iterator
    typedef utils::range<PluginsLoader>::type range;
    typedef iterator::value_type value_type;

    /**
     * Create plugins loader. If @a serviceMode is set to false, no ServicePluginInterface
     * will be loaded.
     */
    explicit PluginsLoader(QObject *parent = 0);

    /**
     * Loading plugins
     */
    void load(bool serviceMode, const QString& subDir, const QString& baseDir = QString(""));

    /**
     * Return plugin wrappers implementing certain @a Interface.
     */
    template <typename Interface>
    std::vector<value_type> plugins() const
    {
        return m_impl->plugins<Interface>();
    }

    /**
     * Return number of loaded plugins
     */
    size_t size() const;

    /**
     * Add custom plugin @a object (created manually). PluginsLoader takes ownership over @a object
     */
    void add(const QString& name, QObject* object);

    /**
     * Return iterator points to begin
    */
    iterator begin() const;

    /**
     * Return iterator points to end
    */
    iterator end() const;

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
    void skipped(const QString&  plugin);
    /**
     * The signal is emmited when @a plugin failed to load
     */
    void failed(const QString& plugin, const QString& error);

private:
    PluginsLoader_p* m_impl;
};

} // plugins

} // appkit
