#include "threadworker.h"
#include "threadpool.h"
#include "utility.h"
#include "task.h"

void ThreadWorker::operator()()
{
    Task *task;
    while(true)
    {
        {   // acquire lock
            boost::mutex::scoped_lock lock(pool->queue_mutex);

            echo("[ThreadWorker] waiting task\n");
            // look for a work item
            while(!pool->stop && pool->tasks.empty())
            {
                // if there are none wait for notification
                pool->queue_cond.wait(lock);
            }
            echo("[ThreadWorker] got a task\n");

            if(pool->stop) // exit if the pool is stopped
                return;

            // get the task from the queue
            task = pool->tasks.front();
            pool->tasks.pop_front();

        }   // release lock

        // execute the task
        echo("[ThreadWorker] run task\n");
        task->run();
        delete task;
    }
}


