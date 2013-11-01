#ifndef _TASK_TCP_READ_MSG_H_
#define _TASK_TCP_READ_MSG_H_

#include "task.h"

#define TASK_TYPE_TCP_READ_MSG 1

#define MAXBTYE 10

class TaskTcpReadMsg : public Task
{
    private:
        char *line;
    public:
        TaskTcpReadMsg(void *arg);
        ~TaskTcpReadMsg();
        void run();
};

#endif
