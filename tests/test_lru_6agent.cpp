#include <gtest/gtest.h>
#include <climits>
#include "LRUCache.h" // Include your LRUCache implementation header

// Fixture for LRUCache tests
class LRUCacheTest : public ::testing::Test {
protected:
    // You can define helper methods or variables here if needed for setup.
};

// 1. Basic Retrieval (`get`)
TEST_F(LRUCacheTest, UT_LRUCache_Get_BasicRetrieval) {
    LRUCache cache(1);
    cache.put(1, 100);
    EXPECT_EQ(cache.get(1), 100);
}

// 2. Basic Insertion (`put`)
TEST_F(LRUCacheTest, UT_LRUCache_Put_BasicInsertion) {
    LRUCache cache(2);
    cache.put(1, 100);
    cache.put(2, 200);
    EXPECT_EQ(cache.get(1), 100);
    EXPECT_EQ(cache.get(2), 200);
}

// 3. Update Existing Key
TEST_F(LRUCacheTest, UT_LRUCache_Put_UpdateExistingKey) {
    LRUCache cache(1);
    cache.put(1, 100);
    cache.put(1, 200);
    EXPECT_EQ(cache.get(1), 200);
}

// 4. Eviction on Capacity Exceed
TEST_F(LRUCacheTest, UT_LRUCache_Put_EvictionOnCapacityExceed) {
    LRUCache cache(2);
    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);
    EXPECT_EQ(cache.get(1), -1);
    EXPECT_EQ(cache.get(2), 200);
    EXPECT_EQ(cache.get(3), 300);
}

// 5. Mark as Recently Used
TEST_F(LRUCacheTest, UT_LRUCache_Get_MarkAsRecentlyUsed) {
    LRUCache cache(2);
    cache.put(1, 100);
    cache.put(2, 200);
    cache.get(1);
    cache.put(3, 300);
    EXPECT_EQ(cache.get(2), -1);
    EXPECT_EQ(cache.get(1), 100);
    EXPECT_EQ(cache.get(3), 300);
}

// 6. Check Cache Size
TEST_F(LRUCacheTest, UT_LRUCache_Size_CheckCacheSize) {
    LRUCache cache(3);
    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);
    EXPECT_EQ(cache.size(), 3);
}

// 7. Minimum Capacity
TEST_F(LRUCacheTest, UT_LRUCache_Put_MinimumCapacity) {
    LRUCache cache(1);
    cache.put(1, 100);
    cache.put(2, 200);
    EXPECT_EQ(cache.get(1), -1);
    EXPECT_EQ(cache.get(2), 200);
}

// 8. Empty Cache Retrieval
TEST_F(LRUCacheTest, UT_LRUCache_Get_EmptyCacheRetrieval) {
    LRUCache cache(1);
    EXPECT_EQ(cache.get(1), -1);
}

// 9. Zero Capacity
TEST_F(LRUCacheTest, UT_LRUCache_Constructor_ZeroCapacity) {
    EXPECT_THROW(LRUCache cache(0), std::invalid_argument);
}

// 10. Retrieve Non-Existent Key
TEST_F(LRUCacheTest, UT_LRUCache_Get_RetrieveNonExistentKey) {
    LRUCache cache(1);
    EXPECT_EQ(cache.get(999), -1);
}

// 11. Insert Duplicate Key
TEST_F(LRUCacheTest, UT_LRUCache_Put_InsertDuplicateKey) {
    LRUCache cache(1);
    cache.put(1, 100);
    cache.put(1, 200);
    EXPECT_EQ(cache.get(1), 200);
}

// 12. Negative Key or Value
TEST_F(LRUCacheTest, UT_LRUCache_Put_NegativeKeyOrValue) {
    LRUCache cache(1);
    cache.put(-1, -100);
    EXPECT_EQ(cache.get(-1), -100);
}

// 13. Memory Allocation Failure
TEST_F(LRUCacheTest, UT_LRUCache_Constructor_MemoryAllocationFailure) {
    // TODO: Add mock/stub for memory allocation failure
    EXPECT_NO_THROW(LRUCache cache(1000000000));
}

// 14. Invalid Iterator State
TEST_F(LRUCacheTest, UT_LRUCache_Iterator_InvalidState) {
    LRUCache cache(2);
    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300); // Eviction occurs
    EXPECT_EQ(cache.get(1), -1);
    EXPECT_EQ(cache.get(2), 200);
    EXPECT_EQ(cache.get(3), 300);
    // TODO: Add mock/stub to validate iterator state
}

// 15. Concurrency Issues
TEST_F(LRUCacheTest, UT_LRUCache_Concurrency_Issues) {
    // TODO: Add multithreading test for race conditions
}

// 16. Maximum Capacity
TEST_F(LRUCacheTest, UT_LRUCache_Constructor_MaximumCapacity) {
    LRUCache cache(INT_MAX);
    EXPECT_NO_THROW(cache.put(1, 100));
    EXPECT_EQ(cache.get(1), 100);
}

// 17. Negative Capacity Initialization
/*TEST_F(LRUCacheTest, UT_LRUCache_Constructor_NegativeCapacity) {
    EXPECT_THROW(LRUCache cache(-1), std::invalid_argument);
}*/

// 18. Retrieval After Eviction
TEST_F(LRUCacheTest, UT_LRUCache_Get_RetrievalAfterEviction) {
    LRUCache cache(2);
    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300); // Eviction occurs
    EXPECT_EQ(cache.get(1), -1);
    EXPECT_EQ(cache.get(2), 200);
    EXPECT_EQ(cache.get(3), 300);
}

// 19. Update Non-Existent Key
TEST_F(LRUCacheTest, UT_LRUCache_Put_UpdateNonExistentKey) {
    LRUCache cache(2);
    cache.put(999, 999);
    EXPECT_EQ(cache.get(999), 999);
}
