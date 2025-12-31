#include <gtest/gtest.h>
#include "LRUCache.h"

// Test fixture for LRUCache
class LRUCacheTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Optional: Code to set up before each test
    }

    void TearDown() override {
        // Optional: Code to clean up after each test
    }
};

// Test constructor with valid capacity
TEST_F(LRUCacheTest, ConstructorValidCapacity) {
    EXPECT_NO_THROW(LRUCache cache(5));
}

// Test constructor with invalid capacity
TEST_F(LRUCacheTest, ConstructorInvalidCapacity) {
    EXPECT_THROW(LRUCache cache(0), std::invalid_argument);
}

// Test put and get functionality
TEST_F(LRUCacheTest, PutAndGet) {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);

    EXPECT_EQ(cache.get(1), 1);  // Key 1 should return value 1
    EXPECT_EQ(cache.get(2), 2);  // Key 2 should return value 2
    EXPECT_EQ(cache.get(3), -1); // Key 3 does not exist
}

// Test LRU eviction
TEST_F(LRUCacheTest, LRUReplacement) {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(3, 3); // This should evict key 1

    EXPECT_EQ(cache.get(1), -1); // Key 1 should be evicted
    EXPECT_EQ(cache.get(2), 2);  // Key 2 should still exist
    EXPECT_EQ(cache.get(3), 3);  // Key 3 should exist
}

// Test updating an existing key
TEST_F(LRUCacheTest, UpdateExistingKey) {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(1, 10); // Update value of key 1

    EXPECT_EQ(cache.get(1), 10); // Value of key 1 should be updated
    EXPECT_EQ(cache.get(2), 2);  // Key 2 should still exist
}

// Test size of cache
TEST_F(LRUCacheTest, CacheSize) {
    LRUCache cache(3);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(3, 3);

    EXPECT_EQ(cache.size(), 3);

    cache.put(4, 4); // This should evict key 1
    EXPECT_EQ(cache.size(), 3);
    EXPECT_EQ(cache.get(1), -1); // Key 1 should be evicted
}

// Test edge case with capacity of 1
TEST_F(LRUCacheTest, EdgeCaseCapacityOne) {
    LRUCache cache(1);
    cache.put(1, 1);
    EXPECT_EQ(cache.get(1), 1);

    cache.put(2, 2); // This should evict key 1
    EXPECT_EQ(cache.get(1), -1); // Key 1 should be evicted
    EXPECT_EQ(cache.get(2), 2);  // Key 2 should exist
}

// Test edge case with negative keys and values
TEST_F(LRUCacheTest, NegativeKeysAndValues) {
    LRUCache cache(2);
    cache.put(-1, -10);
    cache.put(-2, -20);

    EXPECT_EQ(cache.get(-1), -10); // Negative key -1 should return value -10
    EXPECT_EQ(cache.get(-2), -20); // Negative key -2 should return value -20
    EXPECT_EQ(cache.get(-3), -1);  // Negative key -3 does not exist
}

// Test edge case with large values
TEST_F(LRUCacheTest, LargeValues) {
    LRUCache cache(2);
    cache.put(1, INT_MAX);
    cache.put(2, INT_MIN);

    EXPECT_EQ(cache.get(1), INT_MAX); // Key 1 should return INT_MAX
    EXPECT_EQ(cache.get(2), INT_MIN); // Key 2 should return INT_MIN
}

// Test cache behavior when accessing keys to update LRU order
TEST_F(LRUCacheTest, UpdateLRUOrder) {
    LRUCache cache(3);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(3, 3);

    // Access key 1 to make it most recently used
    EXPECT_EQ(cache.get(1), 1);

    // Add a new key, which should evict key 2 (least recently used)
    cache.put(4, 4);

    EXPECT_EQ(cache.get(2), -1); // Key 2 should be evicted
    EXPECT_EQ(cache.get(1), 1); // Key 1 should still exist
    EXPECT_EQ(cache.get(3), 3); // Key 3 should still exist
    EXPECT_EQ(cache.get(4), 4); // Key 4 should exist
}

// Test case: Ensure LRU order is maintained after multiple puts and gets
TEST_F(LRUCacheTest, ComplexLRUBehavior) {
    LRUCache cache(3);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(3, 3);
    cache.put(4, 4); // Should evict key 1

    EXPECT_EQ(cache.get(1), -1); // Key 1 should be evicted
    EXPECT_EQ(cache.get(2), 2);  // Key 2 should still exist

    // Access key 3 to make it most recently used
    EXPECT_EQ(cache.get(3), 3);

    // Add a new key, which should evict key 4 (least recently used after access)
    cache.put(5, 5);

    EXPECT_EQ(cache.get(4), -1); // Key 4 should be evicted
    EXPECT_EQ(cache.get(3), 3);  // Key 3 should still exist
    EXPECT_EQ(cache.get(5), 5);  // Key 5 should exist
}
