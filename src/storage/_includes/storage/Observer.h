/** @file
 * @brief
 *
 * $Id: $
 */

#pragma once

#include <functional>

namespace storage
{

/**
 * This interface can be used to watch over storage changes (before/after).
 * Default implementation does nothing
 */
class Observer
{
public:
    virtual ~Observer() {}

    // This function is called before data appended to internal reader
    virtual void beforeAppend(int /*from*/, int /*count*/) {}
    // This function is called after data appended to internal reader
    virtual void afterAppend(int /*from*/, int /*count*/) {}

    // This function is called before data removed from internal reader
    virtual void beforeRemove(int /*from*/, int /*count*/) {}
    // This function is called after data removed from internal reader
    virtual void afterRemove(int /*from*/, int /*count*/) {}

    // This function is called before data changed
    virtual void beforeChange(int /*from*/, int /*count*/) {}
    // This function is called after data changed
    virtual void afterChange(int /*from*/, int /*count*/) {}
};

/**
 * One can use the following classes to ensure proper calls of ChangesObserver
 * methods
 */
class ObserverGuard
{
public:
    ObserverGuard(
        Observer& observer,
        int from,
        int count,
        std::function<void(Observer*, int, int)> before,
        std::function<void(Observer*, int, int)> after)
        : m_after(boost::bind(after, &observer, from, count))
    {
        before(&observer, from, count);
    }

    ~ObserverGuard()
    {
        m_after();
    }

private:
    boost::function<void()> m_after;
};

class AppendGuard : ObserverGuard
{
public:
    AppendGuard(Observer& observer, int from, int count)
        : ObserverGuard(
              observer,
              from,
              count,
              boost::bind(&Observer::beforeAppend, _1, _2, _3),
              boost::bind(&Observer::afterAppend, _1, _2, _3))
    {
    }
};

class ChangeGuard : ObserverGuard
{
public:
    ChangeGuard(Observer& observer, int from, int count)
        : ObserverGuard(
              observer,
              from,
              count,
              boost::bind(&Observer::beforeChange, _1, _2, _3),
              boost::bind(&Observer::afterChange, _1, _2, _3))
    {
    }
};

class RemoveGuard : ObserverGuard
{
public:
    RemoveGuard(Observer& observer, int from, int count)
        : ObserverGuard(
              observer,
              from,
              count,
              boost::bind(&Observer::beforeRemove, _1, _2, _3),
              boost::bind(&Observer::afterRemove, _1, _2, _3))
    {
    }
};

} // namespace storage
