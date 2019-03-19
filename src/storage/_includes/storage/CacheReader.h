/** @file
 * @brief       Read from inner vector
 *
 * $Id: $
 */

#pragma once

#include "Reader.h"

#include <vector>

namespace storage
{

/**
 * Class for converting From iterator to ReaderIterator::_inner
 */
template <class Type>
class CacheReaderPrivateIterator : public Iterator<Type>::Private
{
public:
    explicit CacheReaderPrivateIterator(
        typename std::vector<Type>::const_iterator internal)
        : m_internal(std::move(internal))
    {
    }

    virtual const typename Iterator<Type>::value_type&
    dereference() const override
    {
        return *m_internal;
    }

    virtual void increment() override
    {
        ++m_internal;
    }

    virtual bool
    equal(const typename Iterator<Type>::Private& other) const override
    {
        return m_internal
               == static_cast<const CacheReaderPrivateIterator<Type>&>(other)
                      .m_internal;
    }

    std::shared_ptr<typename Iterator<Type>::Private> clone() const override
    {
        return std::make_shared<CacheReaderPrivateIterator<Type>>(m_internal);
    }

private:
    typename std::vector<Type>::const_iterator m_internal;
};

/**
 * Reader for supporting CacheStorage.
 */
template <class T, class K>
class CacheReader : public Reader<T, K>
{
    typedef CacheReaderPrivateIterator<T> Private;

public:
    using Base = Reader<T, K>;
    READER_TYPES(Base)

    using Predicate = std::function<bool(const Type&, const Key&)>;

public:
    // Implementation of Reader interface
    /**
     * Return iterator points to data start
     **/
    virtual iterator begin() const override
    {
        return iterator(std::make_shared<Private>(m_data->begin()));
    }

    /**
     * Return iterator points to data end
     **/
    virtual iterator end() const override
    {
        return iterator(std::make_shared<Private>(m_data->end()));
    }

    /**
     * Size getter
     */
    virtual size_t size() const override
    {
        return m_data->size();
    }

    /**
     * Return true if no data in storage
     */
    virtual bool empty() const override
    {
        return m_data->empty();
    }

    /**
     * Find an element addressed by key
     **/
    virtual Iterator<Type> find(const Key& key) const override
    {
        return Iterator<Type>(std::make_shared<Private>(std::find_if(
            m_data->begin(), m_data->end(), [this, key](auto value) {
                return m_finder(value, key);
            })));
    }

    /**
     * Return iterator points to data end
     **/
    virtual bool contains(const Key& key) const override
    {
        return find(key) != end();
    }

    /**
     * Create CacheReader object
     */
    CacheReader(std::shared_ptr<const Sequence> data, Predicate finder)
        : m_data(std::move(data)), m_finder(std::move(finder))
    {
    }

protected:
    Predicate finder() const
    {
        return m_finder;
    }

private:
    template <typename BaseStorage>
    friend class CacheStorage;
    template <typename Reader>
    friend class CacheStorageNotifier;

    std::shared_ptr<const Sequence> m_data;
    Predicate m_finder;
};

} // namespace storage
