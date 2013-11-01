#ifndef _TASK_H_
#define _TASK_H_

class Task
{
    protected:
        void *arg;
    public:
        Task(void *a) : arg(a) { }
        virtual ~Task() { }
        virtual void run() = 0;
//        void operator ()() { run(); }
};

#endif
