#ifndef _TASK_TCP_WRITE_MSG_H_
#define _TASK_TCP_WRITE_MSG_H_

#include "task.h"

#define TASK_TYPE_TCP_WRITE_MSG 2

class TaskTcpWriteMsg : public Task
{
    public:
        TaskTcpWriteMsg(void *arg) : Task(arg) {}
        void run();
};

#endif
