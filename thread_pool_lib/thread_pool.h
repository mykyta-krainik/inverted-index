#ifndef INVERTED_INDEX_LIB_THREAD_POOL_H
#define INVERTED_INDEX_LIB_THREAD_POOL_H

#include <queue>
#include <utility>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <functional>
#include <any>

#include <thread>
#include <future>
#include <atomic>
#include <shared_mutex>
#include <condition_variable>
#include <memory>

using std::pair;
using std::queue;
using std::unordered_set;
using std::unordered_map;
using std::vector;

using std::function;
using std::any;

using std::thread;
using std::future;
using std::atomic_bool;
using std::atomic_uint;
using std::shared_mutex;
using std::mutex;
using std::condition_variable;
using std::unique_lock;

using std::packaged_task;

using std::runtime_error;

using std::forward;
using std::bind;
using std::packaged_task;

using std::make_shared;
using std::future_status;
using std::any_cast;

using write_lock = unique_lock<mutex>;

using task_id_t = unsigned int;
using task_func_t = packaged_task<any()>;
using task_t = pair<task_id_t, task_func_t>;
using task_queue = queue<task_t>;
using completed_tasks = unordered_set<task_id_t>;
using threads = vector<thread>;
using tasks_futures = unordered_map<task_id_t, future<any>>;

class thread_pool {
public:
    explicit thread_pool(unsigned int threads_num);
    ~thread_pool();

    template<typename F, typename ...Args>
    task_id_t add_task(F&& f, Args&&... args) {
        if (is_shutdown_) {
            throw runtime_error("thread pool is shutdown");
        }

        if (is_adding_task_blocked_) {
            throw runtime_error("adding tasks is blocked");
        }

        auto task_func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        auto task_wrapper = [task_func]() -> any {
            return task_func();
        };
        task_id_t task_id = tasks_num_;
        task_func_t task(task_wrapper);
        future<any> future;

        {
            write_lock lock(tasks_mutex_);

            tasks_.emplace(task_id, std::move(task));
            future = tasks_.back().second.get_future();
            ++tasks_num_;
        }

        {
            write_lock lock(tasks_futures_mutex_);

            tasks_futures_[task_id] = std::move(future);
        }

        tasks_cv_.notify_one();

        return task_id;
    }

    bool is_task_finished(task_id_t task_id);

    void wait_all();
    void wait(task_id_t task_id);
    any wait_and_get(task_id_t task_id);
    future<any> get_future(task_id_t task_id);

    void shutdown();

private:
    threads threads_;
    task_queue tasks_;
    completed_tasks completed_tasks_;
    tasks_futures tasks_futures_;

    mutex tasks_mutex_;
    mutex completed_tasks_mutex_;
    mutex tasks_futures_mutex_;

    condition_variable tasks_cv_;
    condition_variable completed_tasks_cv_;

    atomic_bool is_shutdown_ = false;
    atomic_bool is_adding_task_blocked_ = false;
    atomic_uint tasks_num_ = 0;

    void run();
};

#endif
