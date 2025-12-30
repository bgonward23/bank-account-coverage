#include "simple_thread_pool.h"
#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>
#include <vector>
#include <stdexcept>

// Constructor: zero threads should throw
TEST(SimpleThreadPoolTest, ConstructorThrowsOnZeroThreads) {
    EXPECT_THROW(SimpleThreadPool pool(0), std::invalid_argument);
}

// Single task executes and returns correct result
TEST(SimpleThreadPoolTest, SingleTaskReturnsValue) {
    SimpleThreadPool pool(1);
    auto fut = pool.enqueue([](){ return 42; });
    EXPECT_EQ(fut.get(), 42);
}

// Multiple tasks return correct results
TEST(SimpleThreadPoolTest, MultipleTasksReturnCorrectValues) {
    const int numTasks = 8;
    SimpleThreadPool pool(4);
    std::vector<std::future<int>> futures;
    futures.reserve(numTasks);

    for (int i = 0; i < numTasks; ++i) {
        futures.emplace_back(pool.enqueue([i](){ return i * 3; }));
    }

    for (int i = 0; i < numTasks; ++i) {
        EXPECT_EQ(futures[i].get(), i * 3);
    }
}

// Exception inside task propagates through future
TEST(SimpleThreadPoolTest, ExceptionPropagation) {
    SimpleThreadPool pool(2);
    auto fut = pool.enqueue([]() -> int {
        throw std::runtime_error("task failure");
    });
    EXPECT_THROW(fut.get(), std::runtime_error);
}

// Tasks complete before pool destruction (destructor waits)
TEST(SimpleThreadPoolTest, TasksCompleteBeforeDestruction) {
    std::atomic<int> counter{0};
    const int numTasks = 6;

    {
        SimpleThreadPool pool(3);
        std::vector<std::future<void>> futures;
        futures.reserve(numTasks);

        for (int i = 0; i < numTasks; ++i) {
            futures.emplace_back(pool.enqueue([&counter]() {
                // Simulate some work
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                counter.fetch_add(1, std::memory_order_relaxed);
            }));
        }

        // Wait on futures here to be explicit (not required for this test,
        // but ensures tasks started)
        for (auto &f : futures) {
            f.wait();
        }
        // Pool still in scope; destructor hasn't run yet.
        EXPECT_LE(counter.load(), numTasks);
    } // pool destructor runs here and must wait for any outstanding tasks.

    // After pool destruction, all tasks must have completed
    EXPECT_EQ(counter.load(), numTasks);
}

// Detect concurrency: ensure at least two tasks ran concurrently
TEST(SimpleThreadPoolTest, TasksRunConcurrently) {
    const int numTasks = 8;
    const int poolSize = 4;
    SimpleThreadPool pool(poolSize);

    std::atomic<int> current_running{0};
    std::atomic<int> max_running{0};

    std::vector<std::future<void>> futures;
    futures.reserve(numTasks);

    for (int i = 0; i < numTasks; ++i) {
        futures.emplace_back(pool.enqueue([&](){
            int now = ++current_running;
            // update max
            int prev_max = max_running.load();
            while (now > prev_max && !max_running.compare_exchange_weak(prev_max, now)) {
                // loop until updated or prev_max >= now
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(80));
            --current_running;
        }));
    }

    for (auto &f : futures) f.get();

    // With poolSize==4 and numTasks==8 we should have observed concurrency > 1
    EXPECT_GE(max_running.load(), 2);
    // It is reasonable to expect up to poolSize concurrency observed
    EXPECT_LE(max_running.load(), poolSize);
}

// Captured variables by reference behave as expected
TEST(SimpleThreadPoolTest, CaptureByReference) {
    SimpleThreadPool pool(2);
    int x = 5;
    auto fut = pool.enqueue([&x]() {
        x += 10;
        return x;
    });
    EXPECT_EQ(fut.get(), 15);
    EXPECT_EQ(x, 15);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
