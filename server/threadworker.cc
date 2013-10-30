#include "threadworker.h"
#include "threadpool.h"

void ThreadWorker::operator()()
{
    boost::function0<void> task;
    while(true)
    {
        {   // acquire lock
            boost::mutex::scoped_lock lock(pool->queue_mutex);

            // look for a work item
            while(!pool->stop && pool->tasks.empty())
            {
                // if there are none wait for notification
                pool->queue_cond.wait(lock);
            }

            if(pool->stop) // exit if the pool is stopped
                return;

            // get the task from the queue
            task = pool->tasks.front();
            pool->tasks.pop_front();

        }   // release lock

        // execute the task
        task();
    }
}


