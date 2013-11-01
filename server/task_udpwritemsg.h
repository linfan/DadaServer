#ifndef _TASK_UDP_WRITE_MSG_H_
#define _TASK_UDP_WRITE_MSG_H_

#include "task.h"

#define TASK_TYPE_UDP_WRITE_MSG 4

class TaskUdpWriteMsg : public Task
{
    public:
        TaskUdpWriteMsg(void *arg) : Task(arg) {}
        void run();
};

#endif
