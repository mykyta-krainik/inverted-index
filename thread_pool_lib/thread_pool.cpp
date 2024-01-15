#include "thread_pool.h"
#include <iostream>

thread_pool::thread_pool(unsigned int threads_num) {
    threads_.reserve(threads_num);

    for (unsigned int i = 0; i < threads_num; ++i) {
        threads_.emplace_back(&thread_pool::run, this);
    }
}

void thread_pool::run() {
    while (!is_shutdown_) {
        task_t task;

        {
            write_lock lock(tasks_mutex_);

            tasks_cv_.wait(lock, [this]() {
                return !tasks_.empty() || is_shutdown_ || is_adding_task_blocked_;
            });

            if (is_shutdown_) {
                return;
            }

            if (tasks_.empty() && is_adding_task_blocked_) {
                return;
            }

            if (tasks_.empty()) {
                continue;
            }

            task = std::move(tasks_.front());
            tasks_.pop();
        }

        task.second();

        write_lock lock(completed_tasks_mutex_);

        completed_tasks_.insert(task.first);
        completed_tasks_cv_.notify_all();
    }
}

bool thread_pool::is_task_finished(task_id_t task_id) {
    write_lock lock(completed_tasks_mutex_);

    return completed_tasks_.find(task_id) != completed_tasks_.end();
}

void thread_pool::wait_all() {
    write_lock lock(completed_tasks_mutex_);

    completed_tasks_cv_.wait(lock, [this]() {
        return completed_tasks_.size() == tasks_num_ || is_shutdown_;
    });
}

void thread_pool::wait(task_id_t task_id) {
    write_lock lock(completed_tasks_mutex_);

    completed_tasks_cv_.wait(lock, [this, task_id]() {
        return completed_tasks_.find(task_id) != completed_tasks_.end() || is_shutdown_;
    });
}

any thread_pool::wait_and_get(task_id_t task_id) {
    future<any> task_future;

    {
        write_lock lock(tasks_mutex_);
        auto it = tasks_futures_.find(task_id);

        if (it == tasks_futures_.end()) {
            throw runtime_error("Invalid task ID or task already retrieved");
        }

        task_future = std::move(it->second);
        tasks_futures_.erase(it);
    }

    return task_future.get();
}

future<any> thread_pool::get_future(task_id_t task_id) {
    write_lock lock(tasks_futures_mutex_);

    return std::move(tasks_futures_.at(task_id));
}

void thread_pool::shutdown() {
    is_adding_task_blocked_ = true;

    for (auto& thread : threads_) {
        tasks_cv_.notify_all();
        thread.join();
    }

    is_shutdown_ = true;

    threads_.clear();

    write_lock lock(completed_tasks_mutex_);

    completed_tasks_.clear();
}

thread_pool::~thread_pool() {
    shutdown();
}
