#include <gtest/gtest.h>
#include <future>
#include <thread>
#include <vector>
#include "thread_pool.h"
#include <iostream>

class ThreadPoolTest : public ::testing::Test {
protected:
    thread_pool *pool{};

    void SetUp() override {
        pool = new thread_pool(4);
    }

    void TearDown() override {
        pool->shutdown();
        delete pool;
    }
};

TEST_F(ThreadPoolTest, SingleTaskExecution) {
    auto id = pool->add_task([]() { return 42; });

    auto future = pool->get_future(id);

    auto result = future.get();
    int answer = std::any_cast<int>(result);

    EXPECT_EQ(answer, 42);
}

TEST_F(ThreadPoolTest, MultipleTaskExecution) {
    std::vector<std::future<std::any>> futures;
    for (int i = 0; i < 10; ++i) {
        auto id = pool->add_task([i]() { return i; });
        auto future = pool->get_future(id);

        futures.push_back(std::move(future));
    }
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(std::any_cast<int>(futures[i].get()), i);
    }
}

TEST_F(ThreadPoolTest, ConcurrentTaskAddition) {
    std::vector<std::future<any>> futures;
    std::vector<std::thread> threads;
    const int threads_num = 10;

    threads.reserve(threads_num);

    for (int i = 0; i < threads_num; ++i) {
        threads.emplace_back([this, &futures, i]() {
            auto task_id = pool->add_task([i]() { return i; });
            auto future = pool->get_future(task_id);

            futures.push_back(std::move(future));
        });
    }
    for (auto &th : threads) {
        th.join();
    }
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(std::any_cast<int>(futures[i].get()), i);
    }
}

TEST_F(ThreadPoolTest, ShutdownBehavior) {
    auto task_id = pool->add_task([]() { std::this_thread::sleep_for(std::chrono::seconds(1));
        return 42; });
    auto future = pool->get_future(task_id);

    pool->shutdown();

    EXPECT_EQ(std::any_cast<int>(future.get()), 42);
    EXPECT_THROW(pool->add_task([]() { return 43; }), std::exception);
}

TEST_F(ThreadPoolTest, WaitForSpecificTask) {
    auto task_id = pool->add_task([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return 123;
    });

    pool->wait(task_id);

    auto future = pool->get_future(task_id);

    EXPECT_TRUE(pool->is_task_finished(task_id));
    EXPECT_EQ(std::any_cast<int>(future.get()), 123);
}

TEST_F(ThreadPoolTest, WaitAllTasks) {
    std::vector<std::future<any>> futures;

    for (int i = 0; i < 5; ++i) {
        auto task_id = pool->add_task([i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100 * i));
            return i;
        });

        futures.push_back(pool->get_future(task_id));
    }

    pool->wait_all();

    for (auto &f : futures) {
        EXPECT_TRUE(f.valid() && f.wait_for(std::chrono::seconds(0)) == std::future_status::ready);
    }
}

TEST_F(ThreadPoolTest, WaitAndGetTaskResult) {
    auto task_id = pool->add_task([]() { return 456; });
    auto result = pool->wait_and_get(task_id);

    EXPECT_EQ(std::any_cast<int>(result), 456);
}

//TEST_F(ThreadPoolTest, WaitForNonExistentTask) {
//    EXPECT_THROW(pool->wait(9999), std::exception);
//}

TEST_F(ThreadPoolTest, AddTaskAfterInitiatingShutdown) {
    pool->shutdown();
    EXPECT_THROW(pool->add_task([]() { return 789; }), std::exception);
}

TEST_F(ThreadPoolTest, StressTestWithManyTasks) {
    std::vector<std::future<any>> futures;

    for (int i = 0; i < 1000; ++i) {
        auto task_id = pool->add_task([i]() { return i; });

        futures.push_back(pool->get_future(task_id));
    }

    pool->wait_all();

    for (int i = 0; i < 1000; ++i) {
        EXPECT_EQ(std::any_cast<int>(futures[i].get()), i);
    }
}

//TEST_F(ThreadPoolTest, SubmitEmptyTask) {
//    auto task_id = pool->add_task([]() {});
//
//    future.result.wait();
//
//    EXPECT_TRUE(future.result.valid());
//}

TEST_F(ThreadPoolTest, BurstTaskSubmissionAndImmediateShutdown) {
    for (int i = 0; i < 100; ++i) {
        pool->add_task([i]() { return i; });
    }
    pool->shutdown();
}

TEST_F(ThreadPoolTest, MaximumCapacityTest) {
    for (int i = 0; i < 10000; ++i) {
        pool->add_task([i]() { return i; });
    }
    pool->wait_all();
}

TEST_F(ThreadPoolTest, MultipleShutdownCalls) {
    pool->shutdown();
    EXPECT_NO_THROW(pool->shutdown());
}

TEST_F(ThreadPoolTest, AddTasksAfterCompletion) {
    auto task_id = pool->add_task([]() { return 42; });
    auto future = pool->get_future(task_id);

    EXPECT_NO_THROW(pool->add_task([]() { return 43; }));
}

TEST_F(ThreadPoolTest, LongRunningTasks) {
    auto task_id = pool->add_task([]() { std::this_thread::sleep_for(std::chrono::seconds(5));
        return 42; });
    auto future = pool->get_future(task_id);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    pool->shutdown();
    EXPECT_EQ(std::any_cast<int>(future.get()), 42);
}
