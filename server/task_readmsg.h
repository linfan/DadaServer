#ifndef _TASK_READ_MSG_H_
#define _TASK_READ_MSG_H_

#include "task.h"

#define TASK_TYPE_READ_MSG 1

#define MAXBTYE 10

class TaskReadMsg : public Task
{
    private:
        char *line;
    public:
        TaskReadMsg(void *arg);
        ~TaskReadMsg();
        void run();
};

#endif
