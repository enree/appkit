/** @file
 * @brief       Pseudo-polymorfic iterator for reader class
 *
 * $Id: $
 */

#pragma once

#include <boost/iterator/iterator_facade.hpp>

#include <memory>

namespace storage
{

/**
 * Reader iterator type (const)
 */
template <typename T>
class Iterator
    : public boost::
          iterator_facade<Iterator<T>, const T, boost::forward_traversal_tag>
{

private:
    /**
     * Interface for concrete iterator
     */
    class _inner
    {
    public:
        virtual std::shared_ptr<_inner> clone() const = 0;
        virtual const T& dereference() const = 0;
        virtual void increment() = 0;
        virtual bool equal(const _inner& other) const = 0;
        virtual ~_inner() {}
    };

public:
    typedef T value_type;
    typedef _inner Private;

public:
    /**
     * Empty iterator. Dereference of it cause UB
     */
    Iterator() {}

    explicit Iterator(std::shared_ptr<Private> impl) : m_impl(std::move(impl))
    {
    }

    Iterator(const Iterator& other)
        : m_impl(
              other.m_impl ? other.m_impl->clone() : std::shared_ptr<Private>())
    {
    }

    Iterator& operator=(const Iterator& other)
    {
        if (this != &other)
        {
            m_impl = other.m_impl ? other.m_impl->clone()
                                  : std::shared_ptr<Private>();
        }
        return *this;
    }

private:
    friend class boost::iterator_core_access;

    const value_type& dereference() const
    {
        return m_impl->dereference();
    }

    void increment()
    {
        m_impl->increment();
    }

    bool equal(const Iterator& other) const
    {
        return m_impl->equal(*other.m_impl);
    }

private:
    std::shared_ptr<Private> m_impl;
};

} // namespace storage
