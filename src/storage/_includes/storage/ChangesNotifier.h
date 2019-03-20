/** @file
 * @brief Notifier of data changes
 *
 * @ingroup
 *
 * @copyright  (C) 2016 PKB RIO Design Department
 *
 * $Id: $
 */

#pragma once

#include "coriolis/storage/Observer.h"

#include "coriolis/boost/signals2.h"

namespace rio
{

namespace storage
{

/**
 * @brief The ChangesNotifier sends signals about data changes
 */
class ChangesNotifier
        : public Observer
{
    // first, last
    ADD_SIGNAL(AppendStarted, void(int, int))
    ADD_SIGNAL(AppendFinished, void(int, int))

    // first, last
    ADD_SIGNAL(RemoveStarted, void(int, int))
    ADD_SIGNAL(RemoveFinished, void(int, int))

    // first, last
    ADD_SIGNAL(DataChanged, void(int, int))

public:
    virtual void beforeAppend(int from, int count)
    {
        m_AppendStarted(from, from + count - 1);
    }

    virtual void afterAppend(int from, int count)
    {
        m_AppendFinished(from, from + count - 1);
    }

    virtual void beforeRemove(int from, int count)
    {
        m_RemoveStarted(from, from + count - 1);
    }

    virtual void afterRemove(int from, int count)
    {
        m_RemoveFinished(from, from + count - 1);
    }

    // No need to implement beforeChange
    virtual void afterChange(int from, int count)
    {
        m_DataChanged(from, from + count - 1);
    }
};

} // storage

} // rio
