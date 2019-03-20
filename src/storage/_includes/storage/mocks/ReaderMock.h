/** @file
 * @brief       Reader mock
 *
 * @ingroup RIO_STOKES
 *
 * @copyright  (C) 2016 PKB RIO Design Department
 *
 * $Id: $
 */

#pragma once

#include "storage/Reader.h"

#include <gmock/gmock.h>

namespace rio
{

namespace storage
{

/**
 * Reader mock class
 */
template <typename Type, typename Key>
class ReaderMock : public Reader<Type, Key>
{
public:
    /**
     * Mock Reader methods
     */
    MOCK_CONST_METHOD0_T(begin, void());
    MOCK_CONST_METHOD0_T(end, void());

    MOCK_CONST_METHOD0_T(size, size_t());
    MOCK_CONST_METHOD0_T(empty, bool());
    MOCK_CONST_METHOD1_T(get, Type(const Key&));
};

} // namespace storage

} // namespace rio
