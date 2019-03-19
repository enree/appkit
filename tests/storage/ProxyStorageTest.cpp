/** @file
 * @brief Test suit for proxy storage
 *
 * $Id: $
 */

#include "storage/ProxyStorage.h"
#include "storage/mocks/StorageMock.h"

#include "gqtest"

#include <boost/optional/optional_io.hpp>
#include <string>

namespace storage
{

class ProxyStorageTest : public testing::Test
{
public:
    ProxyStorageTest()
        : m_storage(new StorageMock<std::string, int>())
        , m_proxy(m_storage)
        , m_sample("sample")
    {
    }

protected:
    std::shared_ptr<StorageMock<std::string, int>> m_storage;
    ProxyStorage<Storage<std::string, int>> m_proxy;
    const std::string m_sample;
};

TEST_F(ProxyStorageTest, testInsert)
{
    EXPECT_CALL(*m_storage, insert(m_sample)).Times(testing::Exactly(1));
    m_proxy.insert(m_sample);
}

void doNothingFunction(const std::string&) {}

TEST_F(ProxyStorageTest, testGet)
{
    EXPECT_CALL(*m_storage, get(1)).Times(testing::Exactly(1));
    EXPECT_CALL(*(m_storage->cursorMock()), isInitialized())
        .Times(testing::Exactly(1))
        .WillOnce(testing::Return(true));

    std::string value("locked");
    EXPECT_CALL(*(m_storage->cursorMock()), lock())
        .Times(testing::Exactly(1))
        .WillOnce(testing::ReturnRef(value));

    Mutator<std::string&> cursor = m_proxy.get(1);
    EXPECT_TRUE(cursor);
    EXPECT_EQ("locked", cursor.lock());
}

TEST_F(ProxyStorageTest, testRemove)
{
    EXPECT_CALL(*m_storage, remove(1)).Times(testing::Exactly(1));
    m_proxy.remove(1);
}

TEST_F(ProxyStorageTest, testTake)
{
    const std::string modified("modified");

    EXPECT_CALL(*m_storage, take(1))
        .Times(testing::Exactly(1))
        .WillOnce(testing::Return(modified));

    std::string updated = *m_proxy.take(1);
    EXPECT_EQ(modified, updated);
}

TEST_F(ProxyStorageTest, testClear)
{
    EXPECT_CALL(*m_storage, clear()).Times(testing::Exactly(1));
    m_proxy.clear();
}

} // namespace storage
