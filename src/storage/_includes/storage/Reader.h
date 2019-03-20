/** @file
 * @brief       Read interface for generic storages
 *
 * $Id: $
 */

#pragma once

#include "Iterator.h"

#include "boost/signals2.h"

#include <boost/range.hpp>

#include <utility>
#include <vector>

namespace storage
{

/**
 * Read interface for generic storage.
 * Let us think about Reader as a sequence of objects of type T with additional
 * abilities. It notifies about it's changes so we can easyly interact with
 * external object (like Qt models) by subscribing to Readers signals.
 */
template <typename T, typename K>
class Reader
{
public:
    /**
     * Class typedefs
     */
    using Type = T; // Type of objects being read through Reader
    using Key = K; // Type of key
    using iterator = Iterator<Type>; // Type of iterator
    using Sequence = std::vector<Type>; // Reader sequence (one can use this
                                        // type for filter functions)

    using TypeDiff = std::pair<Type, Type>; // Pair of types: old and new ones
    using TypeDiffs = std::vector<TypeDiff>;

    // The signal is emited when objects have been added
    ADD_SIGNAL_T(Appended, void(const Sequence&))
    // The signal is emited when objects have been removed
    ADD_SIGNAL_T(Removed, void(const Sequence&))
    // The signal is emited when objects have been changed
    ADD_SIGNAL_T(Changed, void(const TypeDiffs&))
    // The signal is emited when all objects have been removed from reader
    ADD_SIGNAL(Cleared, void())

    // This signal is emmited before objects are appended. No objects
    // available yet
    ADD_SIGNAL(AppendStarted, void(int, int))
    // This signal is emmited after objects are appended. Objects
    // are already available
    ADD_SIGNAL(AppendFinished, void(int, int))

    // This signal is emmited before objects are removed. Objects
    // are still availabe
    ADD_SIGNAL(RemoveStarted, void(int, int))
    // This signal is emmited after objects are removed. Objects
    // are not availabe, iterator is invalidated
    ADD_SIGNAL(RemoveFinished, void(int, int))

    // This signal is emmited before data is changed. One can access old data
    // values
    ADD_SIGNAL(DataAboutToBeChanged, void(int, int))
    // This signal is emmited before data has been changed. One can access new
    // data values
    ADD_SIGNAL(DataChanged, void(int, int))
public:
    /**
     * Return iterator points to data start
     **/
    virtual iterator begin() const = 0;

    /**
     * Return iterator points to data end
     **/
    virtual iterator end() const = 0;

    /**
     * Find an element by key
     **/
    virtual iterator find(const Key& key) const = 0;

    /**
     * Check if supplied key exists
     **/
    virtual bool contains(const Key& key) const = 0;

    /**
     * Size getter
     */
    virtual size_t size() const = 0;

    /**
     * Check if there no data in storage
     */
    virtual bool empty() const = 0;

    /**
     * Virtual destructor
     */
    virtual ~Reader() {}
};

} // namespace storage

// Specialize metafunctions for using Reader as boost::range
namespace boost
{

template <class R, class K>
struct range_iterator<const storage::Reader<R, K>>
{
    using type = typename storage::Reader<R, K>::iterator;
};

} // namespace boost

#ifndef READER_TYPES
#define READER_TYPES(Base)                                                     \
public:                                                                        \
    using Type = typename Base::Type;                                          \
    using Key = typename Base::Key;                                            \
    using iterator = typename Base::iterator;                                  \
    using Sequence = typename Base::Sequence;
#endif
