/** @file
 * @brief Cursor object to modify storage
 *
 * $Id: $
 */

#pragma once

#include "exception/General.h"

#include <memory>

namespace storage
{

class MutatorException : public exception::General
{
};

/**
 * Class provide mutating capabilities for Storage
 */
template <typename T>
class Mutator
{
private:
    /**
     * Interface for concrete cursor
     */
    class _inner
    {
    public:
        virtual bool isInitialized() const = 0;
        virtual T lock() = 0;
        virtual void unlock() = 0;
        virtual ~_inner() {}
    };

public:
    using Private = _inner;

public:
    /**
     * Create Cursor object
     */
    explicit Mutator(std::shared_ptr<Private> impl)
        : m_impl(std::move(impl)), m_locked(false)
    {
    }
    /**
     * Convert to bool. Return true if cursor has an element to edit
     */
    operator bool() const
    {
        return m_impl->isInitialized();
    }

    bool isLocked() const
    {
        return m_locked;
    }

    /**
     * Lock object for edit
     */
    T lock()
    {
        m_locked = true;
        return m_impl->lock();
    }

    /**
     * Unlock object being edited
     */
    void unlock()
    {
        m_impl->unlock();
        m_locked = false;
    }

private:
    std::shared_ptr<Private> m_impl;
    bool m_locked;
};

/**
 * Invalid mutator. Return it for objects that cannot be found
 */
template <typename T>
class InvalidPrivateMutator : public Mutator<T>::Private
{
public:
    bool isInitialized() const /*override*/
    {
        return false;
    }

    T lock() /*override*/
    {
        BOOST_THROW_EXCEPTION(MutatorException());
    }

    void unlock() /*override*/
    {
        BOOST_THROW_EXCEPTION(MutatorException());
    }
};

} // namespace storage
