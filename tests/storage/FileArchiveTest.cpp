/** @file
 * @brief FileArchive test suit
 *
 * @ingroup
 *
 * @copyright  (C) 2016 PKB RIO Design Department
 *
 * $Id: $
 */

#include "DummyObject.h"

#include "storage/FileArchive.h"
#include "storage/ValueAttribute.h"
#include "storage/mocks/StorageMock.h"

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/range/algorithm/for_each.hpp>

namespace storage
{

using namespace dummy;

class DummyValueAttribute
    : public storage::ValueAttribute<DummyObject, int, std::string>
{
public:
    /**
     * Retrieve key from passed value
     */
    virtual int key(const DummyObject& value) const
    {
        return value.key();
    }

    /**
     * Convert key to some id. ids should be equal
     * for equal keys and differ for different ones
     */
    virtual std::string id(const int& key) const
    {
        return boost::lexical_cast<std::string>(key);
    }
};

class DummySerializer : public serialization::Serializer<DummyObject>
{
public:
    /**
     * Serialize value to string representation
     */
    virtual std::string serialize(const DummyObject& value) const
    {
        return value.toString();
    }

    /**
     * Deserialize value from string representation
     */
    virtual DummyObject deserialize(const std::string& data) const
    {
        return DummyObject::fromString(data);
    }
};

using Inner = StorageMock<DummyObject, int>;
using InnerPtr = std::shared_ptr<Inner>;
using Archive = FileArchive<Inner>;

class FileArchiveTest : public testing::Test
{
public:
    FileArchiveTest()
        : m_data(testData())
        , m_innerSaver(new Inner)
        , m_saver(
              m_innerSaver,
              "testArchive",
              std::make_unique<DummySerializer>(),
              std::make_unique<DummyValueAttribute>())
    {
        EXPECT_CALL(*m_innerSaver, insertMulti(m_data))
            .Times(testing::Exactly(1));
        m_saver.insertMulti(m_data);
    }

    ~FileArchiveTest()
    {
        m_saver.clear();
    }

protected:
    const std::vector<DummyObject> m_data;

    InnerPtr m_innerSaver;
    Archive m_saver;
};

TEST_F(FileArchiveTest, testLoad)
{
    InnerPtr innerLoader = std::make_shared<Inner>();

    EXPECT_CALL(*innerLoader, insertMulti(testing::_))
        .Times(testing::Exactly(1));

    Archive loader(
        innerLoader,
        "testArchive",
        std::make_unique<DummySerializer>(),
        std::make_unique<DummyValueAttribute>());
}

TEST_F(FileArchiveTest, testClear)
{
    EXPECT_CALL(*m_innerSaver, clear()).Times(testing::AtLeast(1));
    m_saver.clear();
    Archive loader(
        std::make_shared<Inner>(),
        "testArchive",
        std::make_unique<DummySerializer>(),
        std::make_unique<DummyValueAttribute>());
}

TEST_F(FileArchiveTest, testClearAndSave)
{
    EXPECT_CALL(*m_innerSaver, clear()).Times(testing::AtLeast(1));
    m_saver.clear();

    EXPECT_CALL(*m_innerSaver, insert(m_data.at(0))).Times(testing::Exactly(1));
    m_saver.insert(m_data.at(0));

    {
        InnerPtr innerLoader = std::make_shared<Inner>();

        EXPECT_CALL(*innerLoader, insertMulti(testing::_))
            .Times(testing::Exactly(1));

        Archive loader(
            innerLoader,
            "testArchive",
            std::make_unique<DummySerializer>(),
            std::make_unique<DummyValueAttribute>());
    }
}

TEST_F(FileArchiveTest, testSaveAndRemove)
{
    {
        EXPECT_CALL(*m_innerSaver, remove(0)).Times(testing::Exactly(1));
        EXPECT_CALL(*m_innerSaver, remove(2)).Times(testing::Exactly(1));

        m_saver.remove(0);
        m_saver.remove(2);
    }

    {
        InnerPtr innerLoader = std::make_shared<Inner>();

        EXPECT_CALL(*innerLoader, insertMulti(testing::_))
            .Times(testing::Exactly(1));

        Archive loader(
            innerLoader,
            "testArchive",
            std::make_unique<DummySerializer>(),
            std::make_unique<DummyValueAttribute>());
    }
}

TEST_F(FileArchiveTest, testSaveUpdateAndLoad)
{
    DummyObject modified(1, "modified");
    {
        EXPECT_CALL(*(m_innerSaver->cursorMock()), isInitialized())
            .WillRepeatedly(testing::Return(true));

        EXPECT_CALL(*(m_innerSaver->cursorMock()), lock())
            .WillRepeatedly(testing::ReturnRef(modified));

        Mutator<DummyObject&> mutator = m_saver.get(1);
        ASSERT_TRUE(mutator);
        mutator.lock().setValue(modified.value());
    }

    {
        InnerPtr innerLoader = std::make_shared<Inner>();
        Archive loader(
            innerLoader,
            "testArchive",
            std::make_unique<DummySerializer>(),
            std::make_unique<DummyValueAttribute>());

        EXPECT_CALL(*innerLoader, insertMulti(testing::_))
            .Times(testing::Exactly(1));
        loader.sync();
    }
}

} // namespace storage
