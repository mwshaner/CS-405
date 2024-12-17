// Uncomment the next line to use precompiled headers
#include "pch.h"
// uncomment the next line if you do not use precompiled headers
//#include "gtest/gtest.h"
//
// the global test environment setup and tear down
// you should not need to change anything here
class Environment : public ::testing::Environment
{
public:
    ~Environment() override {}

    // Override this to define how to set up the environment.
    void SetUp() override
    {
        //  initialize random seed
        srand(time(nullptr));
    }

    // Override this to define how to tear down the environment.
    void TearDown() override {}
};

// create our test class to house shared data between tests
// you should not need to change anything here
class CollectionTest : public ::testing::Test
{
protected:
    // create a smart point to hold our collection
    std::unique_ptr<std::vector<int>> collection;

    void SetUp() override
    { // create a new collection to be used in the test
        collection.reset(new std::vector<int>);
    }

    void TearDown() override
    { //  erase all elements in the collection, if any remain
        collection->clear();
        // free the pointer
        collection.reset(nullptr);
    }

    // helper function to add random values from 0 to 99 count times to the collection
    void add_entries(int count)
    {
        assert(count > 0);
        for (auto i = 0; i < count; ++i)
            collection->push_back(rand() % 100);
    }
};

// When should you use the EXPECT_xxx or ASSERT_xxx macros?
// Use ASSERT when failure should terminate processing, such as the reason for the test case.
// Use EXPECT when failure should notify, but processing should continue

// Test that a collection is empty when created.
// Prior to calling this (and all other TEST_F defined methods),
//  CollectionTest::StartUp is called.
// Following this method (and all other TEST_F defined methods),
//  CollectionTest::TearDown is called
TEST_F(CollectionTest, CollectionSmartPointerIsNotNull)
{
    // is the collection created
    ASSERT_TRUE(collection);

    // if empty, the size must be 0
    ASSERT_NE(collection.get(), nullptr);
}

// test that a collection is empty when created.
TEST_F(CollectionTest, IsEmptyOnCreate)
{
    // is the collection empty?
    ASSERT_TRUE(collection->empty());

    // if empty, the size must be 0
    ASSERT_EQ(collection->size(), 0);
}

/* Comment this test out to prevent the test from running
 * Uncomment this test to see a failure in the test explorer */
//TEST_F(CollectionTest, AlwaysFail)
//{
//    FAIL();
//}

// test to verify adding a single value to an empty collection
TEST_F(CollectionTest, CanAddToEmptyVector)
{
    // is the collection empty?
    // if empty, the size must be 0
    ASSERT_TRUE(collection->empty());

    ASSERT_EQ(collection->size(), 0);

    add_entries(1);

    // is the collection still empty?
    // if not empty, what must the size be?
    ASSERT_FALSE(collection->empty());

    ASSERT_EQ(collection->size(), 1);
}

// test to verify adding five values to collection
TEST_F(CollectionTest, CanAddFiveValuesToVector)
{
    // get the current size
    size_t initialSize = collection->size();
    EXPECT_EQ(initialSize, 0);

    add_entries(5);

    // is the collection still empty?
    // if not empty, what must the size be?
    ASSERT_FALSE(collection->empty());
    ASSERT_EQ(collection->size(), initialSize + 5);
}

// test to verify that max size is greater than or equal to size for 0, 1, 5, 10 entries
TEST_F(CollectionTest, MaxSizeIsGEQThanCurrentSize)
{
    ASSERT_TRUE(collection->empty());
    ASSERT_EQ(collection->size(), 0);

    ASSERT_TRUE(collection->max_size() >= collection->size());

    add_entries(1);
    ASSERT_TRUE(collection->max_size() >= collection->size());

    add_entries(4);
    ASSERT_TRUE(collection->max_size() >= collection->size());

    add_entries(5);
    ASSERT_TRUE(collection->max_size() >= collection->size());

    EXPECT_TRUE(collection->max_size() >= 10);
}


// test to verify that capacity is greater than or equal to size for 0, 1, 5, 10 entries
TEST_F(CollectionTest, CapacityIsGEQThanCurrentSize)
{
    ASSERT_TRUE(collection->empty());
    ASSERT_EQ(collection->size(), 0);

    ASSERT_TRUE(collection->capacity() >= collection->size());

    add_entries(1);
    ASSERT_TRUE(collection->capacity() >= collection->size());

    add_entries(4);
    ASSERT_TRUE(collection->capacity() >= collection->size());

    add_entries(5);
    ASSERT_TRUE(collection->capacity() >= collection->size());

    EXPECT_GT(collection->capacity(),10);
    EXPECT_LT(collection->capacity(), 20);
}

// test to verify resizing increases the collection
TEST_F(CollectionTest, ResizeIncreasesCollection)
{
    size_t initialSize = collection->size();
    EXPECT_EQ(initialSize, 0);

    collection->resize(1);

    ASSERT_GT(collection->size(), initialSize);
    EXPECT_EQ(collection->size(), initialSize + 1);
}


// test to verify resizing decreases the collection
TEST_F(CollectionTest, ResizeDecreasesCollection)
{
    add_entries(5);
    EXPECT_EQ(collection->size(), 5);

    size_t initialSize = collection->size();
    EXPECT_EQ(initialSize, 5);

    collection->resize(1);

    ASSERT_LT(collection->size(), initialSize);
    EXPECT_EQ(collection->size(), initialSize - 4);
}


// test to verify resizing decreases the collection to zero
TEST_F(CollectionTest, ResizeDecreasesCollectionToZero)
{
    add_entries(5);
    EXPECT_EQ(collection->size(), 5);

    collection->resize(0);
    ASSERT_EQ(collection->size(), 0);
}


// test to verify clear erases the collection
TEST_F(CollectionTest, ClearRemovesAllElements)
{
    add_entries(5);
    EXPECT_EQ(collection->size(), 5);

    collection->clear();
    ASSERT_EQ(collection->size(), 0);
}

// test to verify erase(begin,end) erases the collection
TEST_F(CollectionTest, EraseRemovesAllElements)
{
    add_entries(5);
    EXPECT_EQ(collection->size(), 5);

    collection->erase(collection->begin(), collection->end());
    ASSERT_EQ(collection->size(), 0);
}

// test to verify reserve increases the capacity but not the size of the collection
TEST_F(CollectionTest, ReserveIncreasesCapacityNotSize)
{
    size_t initialSize = collection->size();
    EXPECT_EQ(initialSize, 0);

    size_t initialCapacity = collection->capacity();
    EXPECT_EQ(initialCapacity, 0);

    collection->reserve(5);

    ASSERT_EQ(collection->size(), initialSize);
    ASSERT_GT(collection->capacity(), initialCapacity);
}

// test to verify the std::out_of_range exception is thrown when 
// calling at() with an index out of bounds
// NOTE: This is a negative test
TEST_F(CollectionTest, AtThrowsOutOfRangeException)
{
    ASSERT_EQ(collection->size(), 0);

    ASSERT_THROW(collection->at(5), std::out_of_range);
}

// TODO: Create 2 unit tests of your own to test something on the collection - do 1 positive & 1 negative

// test that the pop_back method removes the last element added to the vector
TEST_F(CollectionTest, PopBackRemovesElement)
{
    size_t initialSize = collection->size();
    EXPECT_EQ(collection->size(), 0);

    add_entries(10);

    ASSERT_EQ(collection->size(), initialSize + 10);
    collection->pop_back();

    ASSERT_EQ(collection->size(), initialSize + 9);
}

// test that reserve throws a length exception with invalid input
TEST_F(CollectionTest, ReserveThrowsLengthError)
{
    EXPECT_EQ(collection->size(), 0);

    ASSERT_THROW(collection->reserve(-1), std::length_error);
}
