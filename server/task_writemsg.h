#ifndef _TASK_WRITE_MSG_H_
#define _TASK_WRITE_MSG_H_

#include "task.h"

#define TASK_TYPE_WRITE_MSG 2

class TaskWriteMsg : public Task
{
    public:
        TaskWriteMsg(void *arg) : Task(arg) {}
        void run();
};

#endif
