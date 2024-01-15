#include <any>

class thread_pool_interface {
public:
    virtual unsigned int add_task() = 0;
    virtual bool is_task_finished(unsigned int task_id) = 0;
    virtual void wait_all() = 0;
    virtual void wait(unsigned int task_id) = 0;
    virtual void shutdown() = 0;
    virtual ~thread_pool_interface() = default;

private:
    virtual void run() = 0;
};
