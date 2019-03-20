/** @file
 * @brief
 *
 * $Id: $
 */

#pragma once

#include "Mutator.h"
#include "Reader.h"

#include <functional>
#include <memory>

namespace storage
{

/**
 * Abstract storage.
 * T is a value type for Storage
 * K is a type for addressing concrete value
 */
template <class T, class K, class E = T&>
class Storage
{

public:
    /**
     * Typedef of template types
     */
    typedef T Type;
    typedef K Key;
    typedef E Edit;

    /**
     * Type of read interface
     */
    using ReadInterface = Reader<Type, Key>;

public:
    /**
     * Insert new object
     */
    virtual void insert(const Type& object) = 0;

    /**
     * Insert several objects at once
     */
    virtual void insertMulti(const std::vector<Type>& objects) = 0;

    /**
     * Return cursor by it's @a key that allows to modify object.
     */
    virtual Mutator<Edit> get(const Key& key) = 0;

    /**
     * Remove object from storage by it's key.
     */
    virtual void remove(const Key& key) = 0;

    /**
     * Remove objects from storage by their keys.
     */
    virtual void removeMulti(const std::vector<Key>& keys) = 0;

    /**
     * Remove object from storage by it's key.
     * Return object being removed if any
     */
    virtual boost::optional<Type> take(const Key& key) = 0;

    /**
     * Remove all objects from storage
     */
    virtual void clear() = 0;

    /**
     * Sync storage objects with inner externals
     */
    virtual void sync() = 0;

    /**
     * Return read interface to storage
     */
    virtual std::shared_ptr<ReadInterface> readInterface() const = 0;

    /**
     * Return read interface to storage
     */
    virtual const ReadInterface& operator()() const
    {
        return *readInterface();
    }

    virtual ~Storage() {}
};

} // namespace storage
