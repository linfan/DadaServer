#ifndef _THREAD_WORKER_H_
#define _THREAD_WORKER_H_

class ThreadPool;

class ThreadWorker
{
private:
    ThreadPool *pool;
public:
    ThreadWorker(ThreadPool *p) : pool(p) {}
    void operator()();
};

#endif
