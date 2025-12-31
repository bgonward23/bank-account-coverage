#include <gtest/gtest.h>
#include <stdexcept>
#include "LRUCache.h"

// Test fixture for LRUCache
class LRUCacheTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Default capacity for tests
        cache = LRUCache(3);
    }

    LRUCache cache;
};

// Test case: Construct with valid capacity
TEST_F(LRUCacheTest, ConstructorValidCapacity) {
    EXPECT_NO_THROW(LRUCache(1));
    EXPECT_NO_THROW(LRUCache(100));
}

// Test case: Construct with zero capacity should throw
TEST_F(LRUCacheTest, ConstructorZeroCapacityThrows) {
    EXPECT_THROW(LRUCache(0), std::invalid_argument);
}

// Test case: Get non-existent key returns -1
TEST_F(LRUCacheTest, GetNonExistentKey) {
    EXPECT_EQ(cache.get(1), -1);
}

// Test case: Put new key and get it
TEST_F(LRUCacheTest, PutAndGet) {
    cache.put(1, 100);
    EXPECT_EQ(cache.get(1), 100);
}

// Test case: Put exceeds capacity, evicts LRU
TEST_F(LRUCacheTest, PutExceedsCapacityEvictsLRU) {
    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);
    // Now cache is full
    cache.put(4, 400); // Should evict key 1 (LRU)

    EXPECT_EQ(cache.get(1), -1); // Evicted
    EXPECT_EQ(cache.get(2), 200);
    EXPECT_EQ(cache.get(3), 300);
    EXPECT_EQ(cache.get(4), 400);
}

// Test case: Accessing existing key updates LRU order
TEST_F(LRUCacheTest, AccessUpdatesLRUOrder) {
    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);

    // Access key 2 -> becomes most recently used
    cache.get(2);

    // Add new key -> should evict key 1 (not 2)
    cache.put(4, 400);

    EXPECT_EQ(cache.get(1), -1); // Evicted
    EXPECT_EQ(cache.get(2), 200); // Still present
    EXPECT_EQ(cache.get(3), 300);
    EXPECT_EQ(cache.get(4), 400);
}

// Test case: Update existing key value
TEST_F(LRUCacheTest, UpdateExistingKey) {
    cache.put(1, 100);
    cache.put(1, 101); // Update value
    EXPECT_EQ(cache.get(1), 101);
}

// Test case: Update existing key updates LRU order
TEST_F(LRUCacheTest, UpdateExistingKeyUpdatesLRU) {
    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);

    // Update key 1 -> becomes most recently used
    cache.put(1, 101);

    // Add new key -> should evict key 2 (LRU)
    cache.put(4, 400);

    EXPECT_EQ(cache.get(1), 101);
    EXPECT_EQ(cache.get(2), -1); // Evicted
    EXPECT_EQ(cache.get(3), 300);
    EXPECT_EQ(cache.get(4), 400);
}

// Test case: Multiple gets on same key
TEST_F(LRUCacheTest, MultipleGetsSameKey) {
    cache.put(1, 100);
    EXPECT_EQ(cache.get(1), 100);
    EXPECT_EQ(cache.get(1), 100);
    EXPECT_EQ(cache.get(1), 100);
}

// Test case: Large keys and values
TEST_F(LRUCacheTest, LargeKeysAndValues) {
    cache.put(INT_MAX, INT_MAX);
    cache.put(INT_MIN, INT_MIN);
    EXPECT_EQ(cache.get(INT_MAX), INT_MAX);
    EXPECT_EQ(cache.get(INT_MIN), INT_MIN);
}

// Test case: Zero as valid value
TEST_F(LRUCacheTest, ZeroValueIsValid) {
    cache.put(1, 0);
    EXPECT_EQ(cache.get(1), 0);
}

// Test case: Negative keys
TEST_F(LRUCacheTest, NegativeKeys) {
    cache.put(-1, 100);
    cache.put(-2, 200);
    EXPECT_EQ(cache.get(-1), 100);
    EXPECT_EQ(cache.get(-2), 200);
}

// Test case: Cache size reporting
TEST_F(LRUCacheTest, SizeReporting) {
    EXPECT_EQ(cache.size(), 0);
    cache.put(1, 100);
    EXPECT_EQ(cache.size(), 1);
    cache.put(2, 200);
    EXPECT_EQ(cache.size(), 2);
    cache.get(1); // Access doesn't change size
    EXPECT_EQ(cache.size(), 2);
    cache.put(3, 300);
    EXPECT_EQ(cache.size(), 3);
    cache.put(4, 400); // Eviction occurs
    EXPECT_EQ(cache.size(), 3);
}

// Test case: Reinsert evicted key
TEST_F(LRUCacheTest, ReinsertEvictedKey) {
    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);
    cache.put(4, 400); // Evicts 1

    EXPECT_EQ(cache.get(1), -1);

    // Reinsert key 1
    cache.put(1, 101);
    EXPECT_EQ(cache.get(1), 101);
    EXPECT_EQ(cache.get(4), 400); // Should evict 2
    EXPECT_EQ(cache.get(2), -1);
}

// Test case: Single element cache
TEST_F(LRUCacheTest, SingleElementCache) {
    LRUCache singleCache(1);
    singleCache.put(1, 100);
    EXPECT_EQ(singleCache.get(1), 100);
    singleCache.put(2, 200); // Evicts 1
    EXPECT_EQ(singleCache.get(1), -1);
    EXPECT_EQ(singleCache.get(2), 200);
}

// Test case: Edge case - capacity 1 operations
TEST_F(LRUCacheTest, CapacityOneOperations) {
    LRUCache singleCache(1);
    singleCache.put(1, 100);
    singleCache.put(1, 101); // Update
    EXPECT_EQ(singleCache.get(1), 101);
    singleCache.put(2, 200); // Evicts 1
    EXPECT_EQ(singleCache.get(1), -1);
    EXPECT_EQ(singleCache.get(2), 200);
}
