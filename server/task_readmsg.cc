#include <sys/socket.h>      // recv() and send()
#include <sys/epoll.h>       // epoll interface
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>          // strerror()
#include <errno.h>

#include "task_readmsg.h"
#include "server_tcpsrv.h"
#include "utility.h"

TaskReadMsg::TaskReadMsg(void *arg) : Task(arg)
{
    line = new char(MAXBTYE);
}

TaskReadMsg::~TaskReadMsg()
{
    delete line;
}

void TaskReadMsg::run()
{
    int n, i;
    TcpPkg* data = (TcpPkg*)arg;
    int fd = data->fd;
    int size;

    echo("[TaskReadMsg] readtask %d handling %d\n", pthread_self(), fd);

    if ((n = recv(fd, line, MAXBTYE, 0)) < 0)
    {
        if (errno == ECONNRESET)
            close(fd);
        echo("[TaskReadMsg] Error: readline failed: [%d] - %s\n", errno, strerror(errno));
        if (data != NULL)
            delete data;
        delete line;
    }
    else if (n == 0)
    {
        close(fd);
        echo("[TaskReadMsg] Error: client closed connection.\n");
        if (data != NULL)
            delete data;
        delete line;
    }
    else
    {
        size = n;
        for (i = 0; i < n; ++i)
        {
            if (line[i] == '\n')
            {
                line[i] = '\0';
                size = i + 1;
            }
        }
        echo("[TaskReadMsg] readtask %d received %d : [%d] %s\n", pthread_self(), fd, size, line);
        if (line[0] != '\0')
        {
            data->srv->TriggerSend(fd, line, size);
        }
    }
}

