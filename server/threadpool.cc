#include "threadworker.h"
#include "threadpool.h"
#include "utility.h"

// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t threads)
{
    stop = false;
    for(size_t i = 0;i < threads;++i)
    {
        boost::thread *t = new boost::thread(ThreadWorker(this));
        workers.push_back(t);
    }
    echo("[ThreadPool] thread pool created\n");
}

// the destructor joins all threads
ThreadPool::~ThreadPool()
{
    // stop all threads
    stop = true;
    queue_cond.notify_all();

    // join them
    for(size_t i = 0; i < workers.size();++i)
        workers[i]->join();

    echo("[ThreadPool] thread pool destroied\n");
}

// add new work item to the pool
//template<class F>
//void ThreadPool::enqueue(F f)
void ThreadPool::enqueue(Task *task)
{
    {
        // acquire lock
        boost::mutex::scoped_lock lock(queue_mutex);

        echo("[ThreadPool] enqueue a task\n");
        // add the task
        tasks.push_back(task);
    } // release lock

    // wake up one thread
    queue_cond.notify_one();
}
