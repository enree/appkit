/** @file
 * @brief Cache storage test suit
 *
 * @ingroup
 *
 * @copyright  (C) 2016 PKB RIO Design Department
 *
 * $Id: $
 */

#include "DummyObject.h"

#include "storage/CacheStorage.h"
#include "boost/SignalTracker.h"

#include "gqtest"

#include <gmock/gmock.h>

namespace storage
{

using namespace dummy;

using DummyStorage = CacheStorage<Storage<ConstDummyPtr, int, DummyPtr>>;

class ReaderNotificationReceiver
{
public:
    MOCK_METHOD1(appended, void(const DummyStorage::Reader::Sequence&));
    MOCK_METHOD1(removed, void(const DummyStorage::Reader::Sequence&));
    MOCK_METHOD1(changed, void(const DummyStorage::Reader::TypeDiffs&));
    MOCK_METHOD0(cleared, void());

    MOCK_METHOD2(appendStarted, void(int, int));
    MOCK_METHOD2(appendFinished, void(int, int));
    MOCK_METHOD2(removeStarted, void(int, int));
    MOCK_METHOD2(removeFinished, void(int, int));
    MOCK_METHOD2(dataAboutToBeChanged, void(int, int));
    MOCK_METHOD2(dataChanged, void(int, int));
};

class CacheStorageTest : public testing::Test
{
public:
    CacheStorageTest() : storage(boost::bind(&DummyObject::key, _1) == _2)
    {
        const DummyStorage::ReadInterface& readInterface = storage();

        readInterface.onAppended(
            boost::bind(&ReaderNotificationReceiver::appended, &receiver, _1),
            tracker());
        readInterface.onRemoved(
            boost::bind(&ReaderNotificationReceiver::removed, &receiver, _1),
            tracker());
        readInterface.onChanged(
            boost::bind(&ReaderNotificationReceiver::changed, &receiver, _1),
            tracker());
        readInterface.onCleared(
            boost::bind(&ReaderNotificationReceiver::cleared, &receiver),
            tracker());

        readInterface.onAppendStarted(
            boost::bind(
                &ReaderNotificationReceiver::appendStarted, &receiver, _1, _2),
            tracker());
        readInterface.onAppendFinished(
            boost::bind(
                &ReaderNotificationReceiver::appendFinished, &receiver, _1, _2),
            tracker());
        readInterface.onRemoveStarted(
            boost::bind(
                &ReaderNotificationReceiver::removeStarted, &receiver, _1, _2),
            tracker());
        readInterface.onRemoveFinished(
            boost::bind(
                &ReaderNotificationReceiver::removeFinished, &receiver, _1, _2),
            tracker());
        readInterface.onDataAboutToBeChanged(
            boost::bind(
                &ReaderNotificationReceiver::dataAboutToBeChanged,
                &receiver,
                _1,
                _2),
            tracker());
        readInterface.onDataChanged(
            boost::bind(
                &ReaderNotificationReceiver::dataChanged, &receiver, _1, _2),
            tracker());
    }

protected:
    DummyStorage storage;
    ReaderNotificationReceiver receiver;
    boostsignals::SignalTracker tracker;
};

TEST_F(CacheStorageTest, testInsertAndRemove)
{
    auto dummyOne = std::make_shared<DummyObject>(1, "one");
    auto dummyTwo = std::make_shared<DummyObject>(2, "two");

    EXPECT_CALL(receiver, appended(DummyStorage::Reader::Sequence(1, dummyOne)))
        .Times(testing::Exactly(1));
    EXPECT_CALL(receiver, appended(DummyStorage::Reader::Sequence(1, dummyTwo)))
        .Times(testing::Exactly(1));

    EXPECT_CALL(receiver, removed(testing::_)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, changed(testing::_)).Times(testing::Exactly(0));
    EXPECT_CALL(receiver, cleared()).Times(testing::Exactly(0));

    EXPECT_CALL(receiver, appendStarted(0, 0)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, appendStarted(1, 1)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, appendFinished(0, 0)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, appendFinished(1, 1)).Times(testing::Exactly(1));

    EXPECT_CALL(receiver, removeStarted(0, 0)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, removeFinished(0, 0)).Times(testing::Exactly(1));

    EXPECT_CALL(receiver, dataAboutToBeChanged(testing::_, testing::_))
        .Times(testing::Exactly(0));
    EXPECT_CALL(receiver, dataChanged(testing::_, testing::_))
        .Times(testing::Exactly(0));

    storage.insert(dummyOne);
    storage.insert(dummyTwo);

    ASSERT_EQ(2UL, storage().size());
    EXPECT_TRUE(storage().contains(1));
    EXPECT_TRUE(storage().contains(2));

    storage.remove(1);
    EXPECT_EQ(1UL, storage().size());
    EXPECT_FALSE(storage().contains(1));
    EXPECT_TRUE(storage().contains(2));
}

TEST_F(CacheStorageTest, testInsertAndClear)
{
    EXPECT_CALL(receiver, appended(testing::_)).Times(testing::Exactly(2));
    EXPECT_CALL(receiver, removed(testing::_)).Times(testing::Exactly(0));
    EXPECT_CALL(receiver, changed(testing::_)).Times(testing::Exactly(0));
    EXPECT_CALL(receiver, cleared()).Times(testing::Exactly(1));

    EXPECT_CALL(receiver, appendStarted(0, 0)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, appendStarted(1, 1)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, appendFinished(0, 0)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, appendFinished(1, 1)).Times(testing::Exactly(1));

    EXPECT_CALL(receiver, removeStarted(0, 1)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, removeFinished(0, 1)).Times(testing::Exactly(1));

    EXPECT_CALL(receiver, dataAboutToBeChanged(testing::_, testing::_))
        .Times(testing::Exactly(0));
    EXPECT_CALL(receiver, dataChanged(testing::_, testing::_))
        .Times(testing::Exactly(0));

    storage.insert(std::make_shared<DummyObject>(1, "one"));
    storage.insert(std::make_shared<DummyObject>(2, "two"));

    EXPECT_EQ(2UL, storage().size());

    storage.clear();
    EXPECT_TRUE(storage().empty());
}

TEST_F(CacheStorageTest, testInsertAndUpdate)
{
    EXPECT_CALL(receiver, appended(testing::_)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, removed(testing::_)).Times(testing::Exactly(0));
    EXPECT_CALL(receiver, changed(testing::_)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, cleared()).Times(testing::Exactly(0));

    EXPECT_CALL(receiver, appendStarted(0, 0)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, appendFinished(0, 0)).Times(testing::Exactly(1));

    EXPECT_CALL(receiver, removeStarted(testing::_, testing::_))
        .Times(testing::Exactly(0));
    EXPECT_CALL(receiver, removeFinished(testing::_, testing::_))
        .Times(testing::Exactly(0));

    EXPECT_CALL(receiver, dataAboutToBeChanged(0, 0))
        .Times(testing::Exactly(1));
    EXPECT_CALL(receiver, dataChanged(0, 0)).Times(testing::Exactly(1));

    storage.insert(std::make_shared<DummyObject>(1, "one"));

    auto object = storage.get(1);
    ASSERT_TRUE(object);
    object.lock()->setValue("modified");

    ASSERT_EQ(1UL, storage().size());
    EXPECT_EQ("modified", (*storage().begin())->value());
}

TEST_F(CacheStorageTest, testMultiOperations)
{
    EXPECT_CALL(receiver, appended(testing::_)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, removed(testing::_)).Times(testing::Exactly(1));

    EXPECT_CALL(receiver, appendStarted(0, 3)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, appendFinished(0, 3)).Times(testing::Exactly(1));

    EXPECT_CALL(receiver, removeStarted(0, 0)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, removeFinished(0, 0)).Times(testing::Exactly(1));

    EXPECT_CALL(receiver, removeStarted(1, 1)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, removeFinished(1, 1)).Times(testing::Exactly(1));

    EXPECT_CALL(receiver, removeStarted(3, 3)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, removeFinished(3, 3)).Times(testing::Exactly(1));

    EXPECT_CALL(receiver, dataAboutToBeChanged(testing::_, testing::_))
        .Times(testing::Exactly(0));
    EXPECT_CALL(receiver, dataChanged(testing::_, testing::_))
        .Times(testing::Exactly(0));

    std::vector<DummyStorage::Type> objects;
    objects.push_back(std::make_shared<DummyObject>(1, "uno"));
    objects.push_back(std::make_shared<DummyObject>(2, "dos"));
    objects.push_back(std::make_shared<DummyObject>(3, "tres"));
    objects.push_back(std::make_shared<DummyObject>(4, "cuatro"));

    storage.insertMulti(objects);
    EXPECT_EQ(4UL, storage().size());

    std::vector<DummyStorage::Key> keys;
    keys.push_back(1);
    keys.push_back(2);
    keys.push_back(4);
    storage.removeMulti(keys);
    EXPECT_EQ(1UL, storage().size());
}

TEST_F(CacheStorageTest, testInsertAndUpdateNotExistingKey)
{
    EXPECT_CALL(receiver, appended(testing::_)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, removed(testing::_)).Times(testing::Exactly(0));
    EXPECT_CALL(receiver, changed(testing::_)).Times(testing::Exactly(0));
    EXPECT_CALL(receiver, cleared()).Times(testing::Exactly(0));

    storage.insert(std::make_shared<DummyObject>(1, "one"));

    Mutator<DummyPtr> object = storage.get(2);
    EXPECT_FALSE(object);
    EXPECT_THROW(object.lock(), MutatorException);
}

TEST_F(CacheStorageTest, testCollectiveInsertAndRemove)
{
    std::vector<std::shared_ptr<const DummyObject>> objects;
    objects.push_back(std::make_shared<const DummyObject>(1, "one"));
    objects.push_back(std::make_shared<const DummyObject>(2, "two"));
    objects.push_back(std::make_shared<const DummyObject>(3, "three"));

    EXPECT_CALL(receiver, appended(testing::_)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, removed(testing::_)).Times(testing::Exactly(1));
    EXPECT_CALL(receiver, changed(testing::_)).Times(testing::Exactly(0));
    EXPECT_CALL(receiver, cleared()).Times(testing::Exactly(0));

    storage.insertMulti(objects);

    ASSERT_EQ(3UL, storage().size());
    EXPECT_TRUE(storage().contains(1));
    EXPECT_TRUE(storage().contains(2));
    EXPECT_TRUE(storage().contains(3));

    std::vector<int> keys;
    keys.push_back(1);
    keys.push_back(3);

    storage.removeMulti(keys);

    EXPECT_EQ(1UL, storage().size());
    EXPECT_FALSE(storage().contains(1));
    EXPECT_TRUE(storage().contains(2));
    EXPECT_FALSE(storage().contains(3));
}

struct TestType
{
    int key;
    std::string value;
};

typedef CacheStorage<Storage<TestType, int, TestType&>> ValueStorage;

TEST(TestValueStorage, testModify)
{
    TestType initial = { 0, "initial" };

    ValueStorage storage(boost::bind(&TestType::key, _1) == _2);
    storage.insert(initial);

    EXPECT_EQ(storage.readInterface()->find(0)->value, "initial");
    storage.get(0).lock().value = "modified";
    EXPECT_EQ(1UL, storage.readInterface()->size());

    EXPECT_EQ(storage.readInterface()->find(0)->value, "modified");
}

} // namespace storage
