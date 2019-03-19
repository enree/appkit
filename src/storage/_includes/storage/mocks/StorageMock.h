/** @file
 * @brief       Storage mock
 *
 * @ingroup RIO_STOKES
 *
 * @copyright  (C) 2016 PKB RIO Design Department
 *
 * $Id: $
 */

#pragma once

#include "storage/Storage.h"

#include <gmock/gmock.h>

#include <boost/make_shared.hpp>

namespace storage
{

/**
 * Mutator::Private mock class
 */
template <class T>
class MutatorMock : public Mutator<T>::Private
{
    typedef T Type;

public:
    MOCK_CONST_METHOD0_T(isInitialized, bool());
    MOCK_METHOD0_T(lock, Type());
    MOCK_METHOD0_T(unlock, void());
};

/**
 * Storage mock class
 */
template <typename T, typename K, typename E = T&>
class StorageMock : public Storage<T, K, E>
{
public:
    StorageMock() : m_cursorMock(new MutatorMock<E>)
    {
        ON_CALL(*this, get(testing::_))
            .WillByDefault(testing::Return(Mutator<E>(m_cursorMock)));
    }

    /**
     * Mock storage methods
     **/
    MOCK_METHOD1_T(insert, void(const T&));
    MOCK_METHOD1_T(insertMulti, void(const std::vector<T>&));
    MOCK_METHOD1_T(get, Mutator<typename Storage<T, K>::Edit>(const K&));
    MOCK_METHOD1_T(remove, void(const K&));
    MOCK_METHOD1_T(removeMulti, void(const std::vector<K>&));
    MOCK_METHOD1_T(take, boost::optional<T>(const K&));
    MOCK_METHOD0_T(clear, void());
    MOCK_METHOD0_T(sync, void());
    MOCK_CONST_METHOD0_T(
        readInterface,
        std::shared_ptr<typename Storage<T, K>::ReadInterface>());

    std::shared_ptr<MutatorMock<E>> cursorMock() const
    {
        return m_cursorMock;
    }

private:
    std::shared_ptr<MutatorMock<E>> m_cursorMock;
};

} // namespace storage
