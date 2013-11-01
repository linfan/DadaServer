#ifndef _TASK_UDP_READ_MSG_H_
#define _TASK_UDP_READ_MSG_H_

#include "task.h"

#define TASK_TYPE_UDP_READ_MSG 3

//#define MAXBTYE 10

class TaskUdpReadMsg : public Task
{
    private:
        //char *line;
    public:
        TaskUdpReadMsg(void *arg);
        ~TaskUdpReadMsg();
        void run();
};

#endif
