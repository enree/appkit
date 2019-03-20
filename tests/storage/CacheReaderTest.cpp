/** @file
 * @brief Cache reader test suit
 *
 * $Id: $
 */

#include "DummyObject.h"

#include "storage/CacheReader.h"

#include "gqtest"

namespace storage
{

using namespace dummy;

using DummyReader = CacheReader<ConstDummyPtr, int>;

TEST(CacheReaderTest, testEmpty)
{
    auto data = std::make_shared<std::vector<ConstDummyPtr>>();

    DummyReader reader(data, [](auto object, auto key) {
        return object->key() == key;
    });

    EXPECT_EQ(0UL, reader.size());
    EXPECT_TRUE(reader.empty());
    EXPECT_TRUE(reader.begin() == reader.end());
    EXPECT_FALSE(reader.contains(1));
    EXPECT_TRUE(reader.find(1) == reader.end());
}

TEST(CacheReaderTest, testNonEmpty)
{
    auto data = std::make_shared<std::vector<ConstDummyPtr>>();
    data->push_back(std::make_shared<DummyObject>(1, "one"));
    data->push_back(std::make_shared<DummyObject>(2, "two"));

    DummyReader reader(data, [](auto object, auto key) {
        return object->key() == key;
    });

    EXPECT_EQ(2UL, reader.size());
    EXPECT_FALSE(reader.empty());

    DummyReader::iterator begin = reader.begin();

    ASSERT_TRUE(begin != reader.end());
    EXPECT_EQ(1, (*begin)->key());

    ++begin;
    ASSERT_TRUE(begin != reader.end());
    EXPECT_EQ(2, (*begin)->key());

    ++begin;
    ASSERT_TRUE(begin == reader.end());
}

TEST(CacheReaderTest, testForeach)
{
    auto data = std::make_shared<std::vector<DummyObject>>();
    data->push_back(DummyObject(1, "value"));
    data->push_back(DummyObject(2, "value"));

    using DummyReader = CacheReader<ConstDummyPtr, int>;

    auto reader = std::make_shared<CacheReader<DummyObject, int>>(
        data, boost::bind(&DummyObject::key, _1) == _2);

    for (const auto& dummy: *reader)
    {
        EXPECT_EQ("value", dummy.value());
    }
}

TEST(CacheReaderTest, testIteratorCopyCtr)
{
    auto data = std::make_shared<std::vector<ConstDummyPtr>>();
    data->push_back(std::make_shared<DummyObject>(1, "one"));
    data->push_back(std::make_shared<DummyObject>(2, "two"));

    DummyReader reader(data, [](auto object, auto key) {
        return object->key() == key;
    });

    ASSERT_EQ(2UL, reader.size());

    DummyReader::iterator begin = reader.begin();
    DummyReader::iterator beginCopy = begin;

    ++beginCopy;
    EXPECT_EQ(1, (*begin)->key());
    EXPECT_EQ(2, (*beginCopy)->key());
}

TEST(CacheReaderTest, testIteratorAssignment)
{
    auto data = std::make_shared<std::vector<ConstDummyPtr>>();
    data->push_back(std::make_shared<DummyObject>(1, "one"));
    data->push_back(std::make_shared<DummyObject>(2, "two"));

    DummyReader reader(data, [](auto object, auto key) {
        return object->key() == key;
    });

    ASSERT_EQ(2UL, reader.size());

    DummyReader::iterator begin = reader.begin();
    DummyReader::iterator beginAssigned;
    beginAssigned = begin;

    ++beginAssigned;
    EXPECT_EQ(1, (*begin)->key());
    EXPECT_EQ(2, (*beginAssigned)->key());
}

} // namespace storage
