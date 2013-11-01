#include <sys/socket.h>      // recv() and send()
#include <sys/epoll.h>       // epoll interface
#include <unistd.h>
#include <stdio.h>
#include <string.h>          // strerror()
#include <errno.h>

#include "task_tcpreadmsg.h"
#include "server_tcpsrv.h"
#include "utility.h"

TaskTcpReadMsg::TaskTcpReadMsg(void *arg) : Task(arg)
{
    TRACE_FUNC_BEGIN
    line = new char(MAXBTYE + 1);
    line[MAXBTYE] = '\0';
    TRACE_FUNC_LEAVE
}

TaskTcpReadMsg::~TaskTcpReadMsg()
{
    TRACE_FUNC_BEGIN
    delete line;
    TRACE_FUNC_LEAVE
}

void TaskTcpReadMsg::run()
{
    TRACE_FUNC_BEGIN
    int n, i;
    TcpPkg* rdata = (TcpPkg*)arg;
    int fd = rdata->fd;
    int size;

    echo("[TaskTcpReadMsg] handling %d\n", fd);

    line[0] = '\0';
    if ((n = recv(fd, line, MAXBTYE, 0)) < 0)
    {
        if (errno == ECONNRESET)
            close(fd);
        echo("[TaskTcpReadMsg] Error: readline failed: [err %d] - %s\n", errno, strerror(errno));
        if (rdata != NULL)
            delete rdata;
    }
    else if (n == 0)
    {
        close(fd);
        echo("[TaskTcpReadMsg] Error: client closed connection.\n");
        if (rdata != NULL)
            delete rdata;
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
        echo("[TaskTcpReadMsg] received %d : [%d byte] %s\n", fd, size, line);
        if (line[0] != '\0')
        {
            if (!rdata->srv->TriggerSend(fd, line, size))
                echo("[TaskTcpReadMsg] Error: trigger send fail.\n");
        }
    }
    TRACE_FUNC_LEAVE
}

