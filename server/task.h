#ifndef _TASK_H_
#define _TASK_H_

class Task
{
    private:
        void *arg;
    protected:
        virtual void run() = 0;
    public:
        Task(void *a) : arg(a) { }
        void operator ()() { run(); }
};

#endif
