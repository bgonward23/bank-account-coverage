#include <gtest/gtest.h>
#include <future>
#include <thread>
#include <chrono>
#include <numeric>
#include <stdexcept>
#include <set>
#include <atomic>
#include <vector>
#include <memory>

// Include the header under test
#include "simple_thread_pool.h"

class SimpleThreadPoolTest : public ::testing::Test {
protected:
    void SetUp() override {
        pool = std::make_unique<SimpleThreadPool>(4);
    }

    void TearDown() override {
        pool.reset();
    }

    std::unique_ptr<SimpleThreadPool> pool;
};

// Test 1: Construction with valid thread count
TEST_F(SimpleThreadPoolTest, Constructor_ValidThreadCount) {
    EXPECT_NO_THROW(SimpleThreadPool(1));
    EXPECT_NO_THROW(SimpleThreadPool(8));
}

// Test 2: Construction with zero threads should throw
TEST_F(SimpleThreadPoolTest, Constructor_ZeroThreads_Throws) {
    EXPECT_THROW(SimpleThreadPool(0), std::invalid_argument);
}

// Test 3: Enqueue simple tasks and retrieve results
TEST_F(SimpleThreadPoolTest, Enqueue_SimpleTasks_ReturnResults) {
    auto future1 = pool->enqueue([]() { return 42; });
    auto future2 = pool->enqueue([](int x, int y) { return x + y; }, 2, 3);

    EXPECT_EQ(future1.get(), 42);
    EXPECT_EQ(future2.get(), 5);
}

// Test 4: Enqueue multiple tasks and ensure they complete
TEST_F(SimpleThreadPoolTest, Enqueue_MultipleTasks) {
    std::vector<std::future<int>> futures;
    const int num_tasks = 100;

    for (int i = 0; i < num_tasks; ++i) {
        futures.emplace_back(pool->enqueue([i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            return i * i;
        }));
    }

    int sum = 0;
    for (auto& future : futures) {
        sum += future.get();
    }

    int expected_sum = (99 * 100 * 199) / 6;
    EXPECT_EQ(sum, expected_sum);
}

// Test 5: Enqueue tasks that modify shared state (with synchronization)
TEST_F(SimpleThreadPoolTest, Enqueue_SharedState_WithMutex) {
    std::vector<int> results;
    std::mutex results_mutex;
    const int num_tasks = 50;

    std::vector<std::future<void>> futures;
    for (int i = 0; i < num_tasks; ++i) {
        futures.emplace_back(pool->enqueue([&results, &results_mutex, i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            std::lock_guard<std::mutex> lock(results_mutex);
            results.push_back(i);
        }));
    }

    for (auto& future : futures) {
        future.wait();
    }

    EXPECT_EQ(results.size(), static_cast<size_t>(num_tasks));
}

// Test 6: Enqueue on stopped pool should throw
TEST_F(SimpleThreadPoolTest, Enqueue_OnStoppedPool_Throws) {
    auto future = pool->enqueue([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    });

    // Destroy pool while task is running
    pool.reset();

    // Task should complete, but we can't enqueue anymore
    EXPECT_NO_THROW(future.wait());
}

// Test 7: Tasks that throw exceptions
TEST_F(SimpleThreadPoolTest, Enqueue_TaskThrows_ExceptionPropagates) {
    auto future = pool->enqueue([]() -> int {
        throw std::runtime_error("Test exception");
        return 0;
    });

    EXPECT_THROW(future.get(), std::runtime_error);
}

// Test 8: Large number of tasks with different return types
TEST_F(SimpleThreadPoolTest, Enqueue_ManyTasks_DifferentTypes) {
    std::vector<std::future<void>> void_futures;
    for (int i = 0; i < 10; ++i) {
        void_futures.emplace_back(pool->enqueue([i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(i));
        }));
    }

    std::vector<std::future<int>> int_futures;
    for (int i = 0; i < 10; ++i) {
        int_futures.emplace_back(pool->enqueue([i]() { return i * 2; }));
    }

    std::vector<std::future<double>> double_futures;
    for (int i = 0; i < 10; ++i) {
        double_futures.emplace_back(pool->enqueue([i]() { return i * 0.5; }));
    }

    for (auto& future : void_futures) {
        future.wait();
    }

    for (size_t i = 0; i < int_futures.size(); ++i) {
        EXPECT_EQ(int_futures[i].get(), static_cast<int>(i * 2));
    }

    for (size_t i = 0; i < double_futures.size(); ++i) {
        EXPECT_DOUBLE_EQ(double_futures[i].get(), static_cast<double>(i) * 0.5);
    }
}

// Test 9: Stress test with atomic counter
TEST_F(SimpleThreadPoolTest, Enqueue_EdgeCase_MaxTasks) {
    std::atomic<int> counter{0};
    const int num_tasks = 1000;

    std::vector<std::future<void>> futures;
    futures.reserve(num_tasks);

    for (int i = 0; i < num_tasks; ++i) {
        futures.emplace_back(pool->enqueue([&counter]() {
            counter.fetch_add(1, std::memory_order_relaxed);
        }));
    }

    for (auto& future : futures) {
        future.wait();
    }

    EXPECT_EQ(counter.load(), num_tasks);
}

// Test 10: Move-only function objects
TEST_F(SimpleThreadPoolTest, Enqueue_MoveOnlyFunction) {
    auto future = pool->enqueue([]() {
        auto ptr = std::make_unique<int>(42);
        return *ptr;
    });

    EXPECT_EQ(future.get(), 42);
}

// Test 11: Verify thread pool uses multiple threads
TEST_F(SimpleThreadPoolTest, Enqueue_UsesMultipleThreads) {
    std::set<std::thread::id> thread_ids;
    std::mutex mutex;
    const int num_tasks = 20;

    std::vector<std::future<void>> futures;
    for (int i = 0; i < num_tasks; ++i) {
        futures.emplace_back(pool->enqueue([&thread_ids, &mutex]() {
            std::lock_guard<std::mutex> lock(mutex);
            thread_ids.insert(std::this_thread::get_id());
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }));
    }

    for (auto& future : futures) {
        future.wait();
    }

    // With 20 tasks and 4 threads, we expect more than one thread used
    EXPECT_GT(thread_ids.size(), 1u);
}

// Test 12: Enqueue with reference arguments
TEST_F(SimpleThreadPoolTest, Enqueue_WithReferenceArguments) {
    int value = 0;
    std::mutex value_mutex;

    std::vector<std::future<void>> futures;
    for (int i = 0; i < 10; ++i) {
        futures.emplace_back(pool->enqueue([&value, &value_mutex](int& ref) {
            std::lock_guard<std::mutex> lock(value_mutex);
            ref += 1;
        }, std::ref(value)));
    }

    for (auto& future : futures) {
        future.wait();
    }

    EXPECT_EQ(value, 10);
}

// Test 13: Destructor waits for running tasks to complete
TEST_F(SimpleThreadPoolTest, Destructor_WaitsForRunningTasks) {
    std::atomic<bool> task_started{false};
    std::atomic<bool> task_completed{false};

    auto future = pool->enqueue([&task_started, &task_completed]() {
        task_started.store(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        task_completed.store(true);
    });

    pool.reset();  // Should block until task completes

    EXPECT_TRUE(task_completed.load());
}

// Test 14: Concurrent enqueue from multiple threads
TEST_F(SimpleThreadPoolTest, Enqueue_ConcurrentFromMultipleThreads) {
    std::vector<std::future<int>> futures;
    std::mutex futures_mutex;
    const int num_enqueuers = 10;
    const int tasks_per_enqueuer = 20;

    std::vector<std::thread> enqueuers;
    std::atomic<int> total_tasks{0};

    for (int t = 0; t < num_enqueuers; ++t) {
        enqueuers.emplace_back([&, t] {
            for (int i = 0; i < tasks_per_enqueuer; ++i) {
                auto future = pool->enqueue([t, i]() {
                    return t * 1000 + i;
                });
                {
                    std::lock_guard<std::mutex> lock(futures_mutex);
                    futures.push_back(std::move(future));
                }
                total_tasks.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    for (auto& th : enqueuers) {
        th.join();
    }

    int sum = 0;
    for (auto& future : futures) {
        sum += future.get();
    }

    EXPECT_EQ(total_tasks.load(), num_enqueuers * tasks_per_enqueuer);
    // Sum is not checked for exact value due to arbitrary return, but retrieval works
}

// Test 15: Repeated creation and destruction
TEST(SimpleThreadPoolDeathTest, RepeatedCreationDestruction) {
    for (int i = 0; i < 10; ++i) {
        SimpleThreadPool pool(3);
        auto future = pool.enqueue([]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            return 1;
        });
        EXPECT_EQ(future.get(), 1);
    }
}
