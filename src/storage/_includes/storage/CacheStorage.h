/** @file
 * @brief Storage with in-memory data
 *
 * @ingroup
 *
 * @copyright  (C) 2016 PKB RIO Design Department
 *
 * $Id: $
 */

#pragma once

#include "CacheReader.h"
#include "Observer.h"
#include "Storage.h"

#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/remove_if.hpp>

namespace storage
{

template <typename T>
void copy(T& to, const T& from)
{
    to = from;
}

template <typename T>
void copy(
    boost::shared_ptr<const T>& to, const boost::shared_ptr<const T>& from)
{
    to = std::make_shared<const T>(*from);
}

template <typename T>
T& constCast(const T& object)
{
    return const_cast<T&>(object);
}

template <typename T>
std::shared_ptr<T> constCast(const std::shared_ptr<const T>& object)
{
    return std::const_pointer_cast<T>(object);
}

template <typename T>
boost::shared_ptr<T> constCast(const boost::shared_ptr<const T>& object)
{
    return boost::const_pointer_cast<T>(object);
}

template <typename Reader>
class CacheStorageNotifier : public Observer
{
public:
    explicit CacheStorageNotifier(const Reader& reader) : m_reader(reader) {}

    void beforeAppend(int from, int count) override
    {
        m_reader.m_AppendStarted(from, from + count - 1);
    }

    void afterAppend(int from, int count) override
    {
        m_reader.m_AppendFinished(from, from + count - 1);
    }

    void beforeRemove(int from, int count) override
    {
        m_reader.m_RemoveStarted(from, from + count - 1);
    }

    void afterRemove(int from, int count) override
    {
        m_reader.m_RemoveFinished(from, from + count - 1);
    }

    void beforeChange(int from, int count) override
    {
        m_reader.m_DataAboutToBeChanged(from, from + count - 1);
    }

    void afterChange(int from, int count) override
    {
        m_reader.m_DataChanged(from, from + count - 1);
    }

private:
    const Reader& m_reader;
};

/**
 * Storage that store data in memory
 */
template <typename BaseStorage>
class CacheStorage : public BaseStorage
{
    template <typename Reader>
    class AnyOfKeyMatch
    {
    public:
        AnyOfKeyMatch(
            typename Reader::Predicate finder,
            const std::vector<typename Reader::Key>& keys)
            : m_finder(finder), m_keys(keys)
        {
        }

        bool operator()(const typename Reader::Type& object)
        {
            return boost::algorithm::
                any_of(m_keys, boost::bind(m_finder, object, _1));
        }

    private:
        typename Reader::Predicate m_finder;
        const std::vector<typename Reader::Key>& m_keys;
    };

public:
    /**
     * Typedef of template types
     */
    using BaseType = BaseStorage;
    using Type = typename BaseType::Type;
    using Key = typename BaseType::Key;
    using Edit = typename BaseType::Edit;
    using Reader = CacheReader<Type, Key>;

private:
    /**
     * Mutator for cache storage
     */
    template <typename StoreType>
    class CacheStoragePrivateMutator : public Mutator<Edit>::Private
    {
    public:
        CacheStoragePrivateMutator(
            StoreType object,
            int index,
            const Reader& reader,
            CacheStorageNotifier<Reader>& notifier)
            : m_object(object)
            , m_oldValue(object)
            , m_index(index)
            , m_reader(reader)
            , m_locked(false)
            , m_notifier(notifier)
        {
        }

        virtual bool isInitialized() const
        {
            return true;
        }

        virtual Edit lock()
        {
            m_notifier.beforeChange(m_index, 1);
            m_locked = true;
            copy(m_oldValue, m_object);
            return constCast(m_object);
        }

        virtual void unlock()
        {
            notify();
            m_locked = false;
        }

        ~CacheStoragePrivateMutator()
        {
            notify();
        }

    private:
        void notify()
        {
            if (m_locked)
            {
                m_notifier.afterChange(m_index, 1);
                m_reader.m_Changed(
                    typename Reader::
                        TypeDiffs(1, std::make_pair(m_oldValue, m_object)));
            }
        }

        StoreType m_object;
        StoreType m_oldValue;

        int m_index;
        const Reader& m_reader;
        bool m_locked;
        CacheStorageNotifier<Reader>& m_notifier;
    };

public:
    explicit CacheStorage(typename Reader::Predicate finder)
        : m_data(new typename Reader::Sequence)
        , m_reader(new Reader(m_data, finder))
        , m_finder(finder)
        , m_notifier(*m_reader)
    {
    }

    /**
     * Insert new object
     */
    void insert(const Type& object) override
    {
        AppendGuard guard(m_notifier, m_data->size(), 1);
        m_data->push_back(object);
        m_reader->m_Appended(typename Reader::Sequence(1, object));
    }
    /**
     * Insert objects
     */
    void insertMulti(const std::vector<Type>& objects) override
    {
        AppendGuard guard(m_notifier, m_data->size(), objects.size());
        m_data->insert(m_data->end(), objects.begin(), objects.end());
        m_reader->m_Appended(objects);
    }

    /**
     * Return CacheStorage mutator
     */
    Mutator<Edit> get(const Key& key) override
    {
        auto it = boost::find_if(*m_data, boost::bind(m_finder, _1, key));
        if (it != m_data->end())
        {
            auto privateMutator
                = std::make_shared<CacheStoragePrivateMutator<Edit>>(
                    constCast(*it),
                    std::distance(m_data->begin(), it),
                    *m_reader,
                    m_notifier);

            return Mutator<Edit>(privateMutator);
        }
        else
        {
            return Mutator<Edit>(
                std::make_shared<InvalidPrivateMutator<Edit>>());
        }
    }

    /**
     * Remove object from storage by it's key
     */
    void remove(const Key& key) override
    {
        auto it = boost::find_if(*m_data, boost::bind(m_finder, _1, key));
        if (it != m_data->end())
        {
            RemoveGuard
                guard(m_notifier, std::distance(m_data->begin(), it), 1);
            Type value = *it;
            m_data->erase(it);

            m_reader->m_Removed(typename Reader::Sequence(1, value));
        }
    }

    /**
     * Remove objects from storage by their keys.
     */
    void removeMulti(const std::vector<Key>& keys) override
    {
        auto first = m_data->begin();
        auto result = first;
        for (int idx = 0; first != m_data->end(); ++first, ++idx)
        {
            if (!AnyOfKeyMatch<Reader>(m_finder, keys)(*first))
            {
                *result++ = *first;
            }
            else
            {
                RemoveGuard guard(m_notifier, idx, 1);
            }
        }

        typename Reader::Sequence removed(result, m_data->end());
        m_data->erase(result, m_data->end());

        m_reader->m_Removed(removed);
    }

    /**
     * Return object from storage by it's key and remove it
     */
    boost::optional<Type> take(const Key& key) override
    {
        auto it = boost::find_if(*m_data, boost::bind(m_finder, _1, key));

        boost::optional<Type> value;
        if (it != m_data->end())
        {
            RemoveGuard
                guard(m_notifier, std::distance(m_data->begin(), it), 1);

            value = boost::optional<Type>(*it);
            m_data->erase(it);
            m_reader->m_Removed(typename Reader::Sequence(1, *value));
        }
        return value;
    }

    /**
     * Remove all objects from storage
     */
    void clear() override
    {
        RemoveGuard guard(m_notifier, 0, m_data->size());
        m_data->clear();
        m_reader->m_Cleared();
    }

    /**
     * Sync storage objects with inner externals
     */
    void sync() override
    {
        // Do nothing here
    }

    /**
     * Return read interface to storage
     */
    std::shared_ptr<typename BaseType::ReadInterface>
    readInterface() const override
    {
        return m_reader;
    }

private:
    std::shared_ptr<typename Reader::Sequence> m_data;
    std::shared_ptr<Reader> m_reader;
    typename Reader::Predicate m_finder;

    CacheStorageNotifier<Reader> m_notifier;
};

} // namespace storage
