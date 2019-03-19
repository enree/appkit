/** @file
 * @brief Proxy storage interface
 *
 * $Id: $
 */

#pragma once

#include "Storage.h"

#include "gsl/gsl_assert"

namespace storage
{

/**
 * Proxy storage. Base storage for creating chain of storages for combining
 * their functionality.
 * One can inherit it's class to create OwnStorage1, OwnStorage2 etc and then
 * combine them in OwnStorage2->OwnStorage1->BaseStorage.
 */
template <typename StorageType>
class ProxyStorage : public StorageType
{
public:
    /**
     * Typedef of template types
     */
    typedef StorageType BaseType;
    typedef typename BaseType::Type Type;
    typedef typename BaseType::Key Key;
    typedef typename BaseType::Edit Edit;

protected:
    /**
     * Implementation of Mutator<Edit>::Private for ProxyStorage.
     */
    class ProxyStoragePrivateMutator : public Mutator<Edit>::Private
    {
    public:
        explicit ProxyStoragePrivateMutator(Mutator<Edit> inner)
            : m_inner(std::move(inner))
        {
        }

        bool isInitialized() const override
        {
            return m_inner.operator bool();
        }

        bool isLocked() const
        {
            return m_inner.isLocked();
        }

        Edit lock() override
        {
            return m_inner.lock();
        }

        void unlock() override
        {
            return m_inner.unlock();
        }

    private:
        Mutator<Edit> m_inner;
    };

public:
    /**
     * Create proxy storage.
     */
    explicit ProxyStorage(std::shared_ptr<BaseType> storage)
        : m_innerStorage(std::move(storage))
    {
        Expects(m_innerStorage);
    }

    /**
     * Insert new object
     */
    void insert(const Type& object) override
    {
        m_innerStorage->insert(object);
    }

    /**
     * Insert new objects
     */
    void insertMulti(const std::vector<Type>& objects) override
    {
        m_innerStorage->insertMulti(objects);
    }

    /**
     * Return inner mutator by it's @a key that allows to modify object.
     */
    Mutator<Edit> get(const Key& address) override
    {
        return m_innerStorage->get(address);
    }

    /**
     * Remove object from storage by it's address
     */
    void remove(const Key& key) override
    {
        return m_innerStorage->remove(key);
    }

    /**
     * Remove object from storage
     */
    void removeMulti(const std::vector<Key>& keys) override
    {
        return m_innerStorage->removeMulti(keys);
    }

    /**
     * Remove object from storage by it's address
     */
    boost::optional<Type> take(const Key& key) override
    {
        return m_innerStorage->take(key);
    }

    /**
     * Remove all objects from storage
     */
    void clear() override
    {
        m_innerStorage->clear();
    }

    /**
     * Sync storage internals
     */
    void sync() override
    {
        m_innerStorage->sync();
    }

    /**
     * Return read interface to storage
     */
    std::shared_ptr<typename BaseType::ReadInterface>
    readInterface() const override
    {
        return m_innerStorage->readInterface();
    }

    virtual ~ProxyStorage() {}

private:
    std::shared_ptr<BaseType> m_innerStorage;
};

} // namespace storage
