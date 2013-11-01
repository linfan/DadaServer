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
    TcpPkg* rdata = (TcpPkg*)arg;
    int fd = rdata->fd;
    int size;

    echo("[TaskReadMsg] handling %d\n", fd);

    if ((n = recv(fd, line, MAXBTYE, 0)) < 0)
    {
        if (errno == ECONNRESET)
            close(fd);
        echo("[TaskReadMsg] Error: readline failed: [err %d] - %s\n", errno, strerror(errno));
        if (rdata != NULL)
            delete rdata;
        delete line;
    }
    else if (n == 0)
    {
        close(fd);
        echo("[TaskReadMsg] Error: client closed connection.\n");
        if (rdata != NULL)
            delete rdata;
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
        echo("[TaskReadMsg] received %d : [%d byte] %s\n", fd, size, line);
        if (line[0] != '\0')
        {
            if (!rdata->srv->TriggerSend(fd, line, size))
                echo("[TaskReadMsg] Error: trigger send fail.\n");
        }
    }
}

