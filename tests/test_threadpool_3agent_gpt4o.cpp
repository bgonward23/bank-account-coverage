#include <gtest/gtest.h>
#include "SimpleThreadPool.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <stdexcept>
#include <atomic>
#include <chrono>

// Test case to check if SimpleThreadPool constructor throws an exception for zero threads
TEST(SimpleThreadPoolTest, ConstructorThrowsOnZeroThreads) {
    EXPECT_THROW(SimpleThreadPool(0), std::invalid_argument);
}

// Test case to check if tasks are executed correctly
TEST(SimpleThreadPoolTest, TasksExecution) {
    SimpleThreadPool pool(4);

    // Test 1: Simple addition
    auto task1 = pool.enqueue([](int a, int b) { return a + b; }, 5, 7);
    EXPECT_EQ(task1.get(), 12);

    // Test 2: String concatenation
    auto task2 = pool.enqueue([](const std::string& str1, const std::string& str2) { return str1 + str2; }, "Hello, ", "world!");
    EXPECT_EQ(task2.get(), "Hello, world!");

    // Test 3: Vector sum
    std::vector<int> vec(1000, 1);
    auto task3 = pool.enqueue([&vec]() { return std::accumulate(vec.begin(), vec.end(), 0); });
    EXPECT_EQ(task3.get(), 1000);
}

// Test case to ensure tasks are executed in parallel
TEST(SimpleThreadPoolTest, ParallelExecution) {
    SimpleThreadPool pool(4);

    std::atomic<int> counter = 0;

    auto task1 = pool.enqueue([&counter]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        counter++;
    });

    auto task2 = pool.enqueue([&counter]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        counter++;
    });

    task1.get();
    task2.get();

    EXPECT_EQ(counter, 2);
}

// Test case to check if enqueue throws exception after pool destruction
TEST(SimpleThreadPoolTest, EnqueueAfterDestruction) {
    std::unique_ptr<SimpleThreadPool> pool = std::make_unique<SimpleThreadPool>(2);

    auto task = pool->enqueue([] { return 42; });
    EXPECT_EQ(task.get(), 42);

    pool.reset(); // Explicitly destroy the thread pool

    EXPECT_THROW(pool->enqueue([] { return 0; }), std::runtime_error);
}

// Test case to check edge cases
TEST(SimpleThreadPoolTest, EdgeCases) {
    SimpleThreadPool pool(2);

    // Test 1: Large number of tasks
    const int num_tasks = 1000;
    std::vector<std::future<int>> results;
    for (int i = 0; i < num_tasks; ++i) {
        results.emplace_back(pool.enqueue([i]() { return i * i; }));
    }

    for (int i = 0; i < num_tasks; ++i) {
        EXPECT_EQ(results[i].get(), i * i);
    }

    // Test 2: Task with no return value
    bool flag = false;
    auto voidTask = pool.enqueue([&flag]() { flag = true; });
    voidTask.get();
    EXPECT_TRUE(flag);
}
