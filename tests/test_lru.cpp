#include "LRUCache.h"
#include <gtest/gtest.h>

// Constructor: zero capacity should throw
TEST(LRUCacheTest, ConstructorThrowsOnZeroCapacity) {
    EXPECT_THROW(LRUCache cache(0), std::invalid_argument);
}

// get on empty cache returns -1 and size remains zero
TEST(LRUCacheTest, GetNonExistentReturnsMinusOne) {
    LRUCache cache(2);
    EXPECT_EQ(cache.get(42), -1);
    EXPECT_EQ(cache.size(), 0u);
}

// Simple put and get
TEST(LRUCacheTest, PutAndGet) {
    LRUCache cache(2);
    cache.put(1, 10);
    EXPECT_EQ(cache.get(1), 10);
    EXPECT_EQ(cache.size(), 1u);
}

// Updating existing key changes value but not size, and moves it to most-recent
TEST(LRUCacheTest, UpdateExistingKeyAndMoveToFront) {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    // Update key 1's value; this should move key 1 to MRU position
    cache.put(1, 10);
    EXPECT_EQ(cache.get(1), 10);
    EXPECT_EQ(cache.size(), 2u);

    // In this state key 1 is MRU, key 2 is LRU; inserting key 3 should evict key 2
    cache.put(3, 3);
    EXPECT_EQ(cache.get(2), -1); // evicted
    EXPECT_EQ(cache.get(1), 10);
    EXPECT_EQ(cache.get(3), 3);
}

// Accessing a key should make it MRU and prevent its eviction
TEST(LRUCacheTest, AccessMovesKeyToFrontPreventEviction) {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    // Access key 1 so it becomes MRU
    EXPECT_EQ(cache.get(1), 1);
    // Insert key 3, should evict key 2
    cache.put(3, 3);
    EXPECT_EQ(cache.get(2), -1);
    EXPECT_EQ(cache.get(1), 1);
    EXPECT_EQ(cache.get(3), 3);
}

// Eviction behavior with capacity 1
TEST(LRUCacheTest, CapacityOneEvictsPrevious) {
    LRUCache cache(1);
    cache.put(1, 100);
    EXPECT_EQ(cache.get(1), 100);
    cache.put(2, 200); // should evict key 1
    EXPECT_EQ(cache.get(1), -1);
    EXPECT_EQ(cache.get(2), 200);
    EXPECT_EQ(cache.size(), 1u);
}

// size reports correctly after inserts and evictions
TEST(LRUCacheTest, SizeReportsCorrectly) {
    LRUCache cache(3);
    EXPECT_EQ(cache.size(), 0u);
    cache.put(1, 1);
    cache.put(2, 2);
    EXPECT_EQ(cache.size(), 2u);
    cache.put(3, 3);
    EXPECT_EQ(cache.size(), 3u);
    // Inserting a fourth should evict one and size stays at capacity
    cache.put(4, 4);
    EXPECT_EQ(cache.size(), 3u);
}

// More complex ordering: ensure the correct LRU is evicted
TEST(LRUCacheTest, ComplexOrderingEviction) {
    LRUCache cache(3);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(3, 3);
    // Current MRU order (front -> back): 3,2,1
    // Access 2 then 1, new order: 1,2,3 (MRU -> LRU)
    EXPECT_EQ(cache.get(2), 2);
    EXPECT_EQ(cache.get(1), 1);
    // Insert 4 -> should evict 3
    cache.put(4, 4);
    EXPECT_EQ(cache.get(3), -1);
    EXPECT_EQ(cache.get(1), 1);
    EXPECT_EQ(cache.get(2), 2);
    EXPECT_EQ(cache.get(4), 4);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
