#ifndef _TASK_READ_MSG_H_
#define _TASK_READ_MSG_H_

#include "task.h"

#define TASK_TYPE_READ_MSG 1

class TaskReadMsg : public Task
{
    public:
        TaskReadMsg(void *arg) : Task(arg) {}
        void run();
};

#endif
