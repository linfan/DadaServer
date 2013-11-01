#include "threadworker.h"
#include "threadpool.h"
#include "utility.h"

// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t threads)
{
    TRACE_FUNC_BEGIN
    stop = false;
    for(size_t i = 0;i < threads;++i)
    {
        boost::thread *t = new boost::thread(ThreadWorker(this));
        workers.push_back(t);
    }
    TRACE_FUNC_LEAVE
}

// the destructor joins all threads
ThreadPool::~ThreadPool()
{
    TRACE_FUNC_BEGIN
    // stop all threads
    stop = true;
    queue_cond.notify_all();

    // join them
    for(size_t i = 0; i < workers.size();++i)
        workers[i]->join();

    TRACE_FUNC_LEAVE
}

// add new work item to the pool
//template<class F>
//void ThreadPool::enqueue(F f)
void ThreadPool::enqueue(Task *task)
{
    TRACE_FUNC_BEGIN
    {
        // acquire lock
        boost::mutex::scoped_lock lock(queue_mutex);

        echo("[ThreadPool] enqueue a task\n");
        // add the task
        tasks.push_back(task);
    } // release lock

    // wake up one thread
    queue_cond.notify_one();
    TRACE_FUNC_LEAVE
}
