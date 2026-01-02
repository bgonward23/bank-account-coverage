#include <gtest/gtest.h>
#include <thread>
#include <future>
#include <stdexcept>
#include "SimpleThreadPool.h" // Assume this is the header file for the thread pool implementation

// Test fixture for SimpleThreadPool
class SimpleThreadPoolTest : public ::testing::Test {
protected:
    // Helper function to simulate a task
    static int simpleTask() {
        return 42;
    }

    // Helper function to simulate a parameterized task
    static int parameterizedTask(int value) {
        return value;
    }
};

// UT_SimpleThreadPool_Constructor_ValidThreadCount
TEST_F(SimpleThreadPoolTest, UT_SimpleThreadPool_Constructor_ValidThreadCount) {
    const int threadCount = 4;
    SimpleThreadPool pool(threadCount);
    ASSERT_NO_THROW(pool);
}

// UT_SimpleThreadPool_Enqueue_SimpleTask
TEST_F(SimpleThreadPoolTest, UT_SimpleThreadPool_Enqueue_SimpleTask) {
    SimpleThreadPool pool(4);
    auto future = pool.enqueue([]() { return 42; });
    ASSERT_EQ(future.get(), 42);
}

// UT_SimpleThreadPool_Enqueue_MultipleTasks
TEST_F(SimpleThreadPoolTest, UT_SimpleThreadPool_Enqueue_MultipleTasks) {
    SimpleThreadPool pool(4);
    const int taskCount = 10;
    std::vector<std::future<int>> futures;

    for (int i = 0; i < taskCount; ++i) {
        futures.push_back(pool.enqueue([i]() { return i; }));
    }

    for (int i = 0; i < taskCount; ++i) {
        ASSERT_EQ(futures[i].get(), i);
    }
}

// UT_SimpleThreadPool_Destructor_ReleasesResources
TEST_F(SimpleThreadPoolTest, UT_SimpleThreadPool_Destructor_ReleasesResources) {
    SimpleThreadPool* pool = new SimpleThreadPool(4);
    delete pool;
    // No explicit assertion; ensure no crash or deadlock occurs
}

// UT_SimpleThreadPool_Constructor_MinimumThreads
TEST_F(SimpleThreadPoolTest, UT_SimpleThreadPool_Constructor_MinimumThreads) {
    const int threadCount = 1;
    SimpleThreadPool pool(threadCount);
    ASSERT_NO_THROW(pool);
}

// UT_SimpleThreadPool_Constructor_MaximumThreads
TEST_F(SimpleThreadPoolTest, UT_SimpleThreadPool_Constructor_MaximumThreads) {
    const int threadCount = 1000;
    SimpleThreadPool pool(threadCount);
    ASSERT_NO_THROW(pool);
}

// UT_SimpleThreadPool_Enqueue_TaskOnEmptyQueue
TEST_F(SimpleThreadPoolTest, UT_SimpleThreadPool_Enqueue_TaskOnEmptyQueue) {
    SimpleThreadPool pool(4);
    auto future = pool.enqueue([]() { return 99; });
    ASSERT_EQ(future.get(), 99);
}

// UT_SimpleThreadPool_Constructor_ZeroThreads
TEST_F(SimpleThreadPoolTest, UT_SimpleThreadPool_Constructor_ZeroThreads) {
    ASSERT_THROW(SimpleThreadPool(0), std::invalid_argument);
}

// UT_SimpleThreadPool_Enqueue_TaskAfterStopped
TEST_F(SimpleThreadPoolTest, UT_SimpleThreadPool_Enqueue_TaskAfterStopped) {
    SimpleThreadPool pool(4);
    pool.~SimpleThreadPool(); // Explicitly call destructor
    ASSERT_THROW(pool.enqueue([]() { return 1; }), std::runtime_error);
}

// UT_SimpleThreadPool_Stress_HighTaskVolume
TEST_F(SimpleThreadPoolTest, UT_SimpleThreadPool_Stress_HighTaskVolume) {
    SimpleThreadPool pool(10);
    const int taskCount = 10000;
    std::vector<std::future<int>> futures;

    for (int i = 0; i < taskCount; ++i) {
        futures.push_back(pool.enqueue([i]() { return i; }));
    }

    for (int i = 0; i < taskCount; ++i) {
        ASSERT_EQ(futures[i].get(), i);
    }
}

// UT_SimpleThreadPool_Stress_RapidEnqueueing
TEST_F(SimpleThreadPoolTest, UT_SimpleThreadPool_Stress_RapidEnqueueing) {
    SimpleThreadPool pool(10);
    const int taskCount = 1000000;

    for (int i = 0; i < taskCount; ++i) {
        pool.enqueue([]() { return; });
    }
    // No explicit assertion; ensure no crash or hang occurs
}

// UT_SimpleThreadPool_Concurrency_MultipleProducers
TEST_F(SimpleThreadPoolTest, UT_SimpleThreadPool_Concurrency_MultipleProducers) {
    SimpleThreadPool pool(10);
    const int producerCount = 5;
    const int tasksPerProducer = 1000;

    std::vector<std::thread> producers;
    for (int i = 0; i < producerCount; ++i) {
        producers.emplace_back([&pool, i, tasksPerProducer]() {
            for (int j = 0; j < tasksPerProducer; ++j) {
                pool.enqueue([i, j, tasksPerProducer]() { return i * tasksPerProducer + j; });
                
            }
        });
    }

    for (auto& producer : producers) {
        producer.join();
    }
    // No explicit assertion; ensure no crash or race condition occurs
}

// UT_SimpleThreadPool_Stop_WithPendingTasks
TEST_F(SimpleThreadPoolTest, UT_SimpleThreadPool_Stop_WithPendingTasks) {
    SimpleThreadPool pool(4);
    pool.enqueue([]() { std::this_thread::sleep_for(std::chrono::seconds(1)); });
    pool.~SimpleThreadPool(); // Explicitly call destructor
    // No explicit assertion; ensure no crash occurs
}

// UT_SimpleThreadPool_ExceptionHandlingInTasks
TEST_F(SimpleThreadPoolTest, UT_SimpleThreadPool_ExceptionHandlingInTasks) {
    SimpleThreadPool pool(4);
    auto future = pool.enqueue([]() { throw std::runtime_error("Task error"); return 0; });
    ASSERT_THROW(future.get(), std::runtime_error);
}

// UT_SimpleThreadPool_IdleThreadPool
TEST_F(SimpleThreadPoolTest, UT_SimpleThreadPool_IdleThreadPool) {
    SimpleThreadPool pool(4);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto future = pool.enqueue([]() { return 123; });
    ASSERT_EQ(future.get(), 123);
}

// UT_SimpleThreadPool_MixedCallableTypes
TEST_F(SimpleThreadPoolTest, UT_SimpleThreadPool_MixedCallableTypes) {
    SimpleThreadPool pool(4);
    auto lambdaFuture = pool.enqueue([]() { return 1; });
    struct Callable {
        int operator()() const { return 2; }
    };
    auto callableFuture = pool.enqueue(Callable());

    ASSERT_EQ(lambdaFuture.get(), 1);
    ASSERT_EQ(callableFuture.get(), 2);
}

// UT_SimpleThreadPool_VariableTaskTimes
TEST_F(SimpleThreadPoolTest, UT_SimpleThreadPool_VariableTaskTimes) {
    SimpleThreadPool pool(4);
    auto shortTask = pool.enqueue([]() { return 1; });
    auto longTask = pool.enqueue([]() { std::this_thread::sleep_for(std::chrono::milliseconds(100)); return 2; });

    ASSERT_EQ(shortTask.get(), 1);
    ASSERT_EQ(longTask.get(), 2);
}
