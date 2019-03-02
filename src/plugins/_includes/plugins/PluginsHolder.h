/**
 * @ingroup RIO_CORIOLIS
 * @file
 * @brief      Plugins holder class.
 * @copyright  (C) 2015 PKB RIO Design Department
 *
 * $Id: $
 */

#pragma once

#include "PluginsLoader.h"

#ifndef Q_MOC_RUN // Protect from BOOST_JOIN error
#include <boost/iterator/iterator_facade.hpp>
#endif

#include <vector>

namespace appkit
{

namespace plugins
{

class PluginsLoader;

template <typename PluginInterface>
struct PluginsHolderType
{
    std::shared_ptr<PluginWrapper> wrapper;

    PluginInterface* operator()() const
    {
        return wrapper->instance<PluginInterface>();
    }

    QObject* instance() const
    {
        return wrapper->instance();
    }
};

/**
 * Plugins iterator type (const)
 */
template <typename PluginInterface>
class PluginsHolderIterator : public boost::iterator_facade<
                                  PluginsHolderIterator<PluginInterface>,
                                  PluginsHolderType<PluginInterface> const,
                                  boost::forward_traversal_tag>
{
public:
    using PluginWrapperPtr = std::shared_ptr<PluginWrapper>;
    using value_type = PluginsHolderType<PluginInterface>;

    explicit PluginsHolderIterator(
        const std::vector<PluginWrapperPtr>::const_iterator& it)
        : m_iterator(it)
    {
    }

private:
    friend class boost::iterator_core_access;

    const value_type& dereference() const
    {
        m_instance.wrapper = *m_iterator;
        return m_instance;
    }

    void increment()
    {
        ++m_iterator;
    }

    bool equal(const PluginsHolderIterator& other) const
    {
        return m_iterator == other.m_iterator;
    }

private:
    mutable value_type m_instance;
    std::vector<PluginWrapperPtr>::const_iterator m_iterator;
};

/**
 * This class takes wrappers
 */
template <typename I>
class PluginsHolder
{
    typedef PluginsHolder<I> this_type;

public:
    typedef PluginsHolderIterator<I> iterator; // Type of iterator
    typedef typename iterator::value_type value_type;

    /**
     * Create plugins holder from loader.
     * This function takes all plugins supports I from loader
     */
    explicit PluginsHolder(PluginsLoader& loader) : m_wrappers(loader.take<I>())
    {
    }

    /**
     * Return iterator points to begin
     */
    iterator begin() const
    {
        return iterator(m_wrappers.begin());
    }

    /**
     * Return iterator points to end
     */
    iterator end() const
    {
        return iterator(m_wrappers.end());
    }

private:
    std::vector<typename iterator::PluginWrapperPtr> m_wrappers;
};

} // namespace plugins

} // namespace appkit
