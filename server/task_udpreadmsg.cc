#include <sys/socket.h>      // recv() and send()
#include <unistd.h>
#include <stdio.h>
#include <string.h>          // strerror()
#include <errno.h>

#include "task_udpreadmsg.h"
#include "server_udpsrv.h"
#include "utility.h"

TaskUdpReadMsg::TaskUdpReadMsg(void *arg) : Task(arg)
{
    TRACE_FUNC_BEGIN
//    line = new char(MAXBTYE);
    TRACE_FUNC_LEAVE
}

TaskUdpReadMsg::~TaskUdpReadMsg()
{
    TRACE_FUNC_BEGIN
//    delete line;
    TRACE_FUNC_LEAVE
}

void TaskUdpReadMsg::run()
{
    TRACE_FUNC_BEGIN
//    int n, i;
//    TcpPkg* rdata = (TcpPkg*)arg;
//    int fd = rdata->fd;
//    int size;
//
//    echo("[TaskUdpReadMsg] handling %d\n", fd);
//
//    if ((n = recv(fd, line, MAXBTYE, 0)) < 0)
//    {
//        if (errno == ECONNRESET)
//            close(fd);
//        echo("[TaskUdpReadMsg] Error: readline failed: [err %d] - %s\n", errno, strerror(errno));
//        if (rdata != NULL)
//            delete rdata;
//        delete line;
//    }
//    else if (n == 0)
//    {
//        close(fd);
//        echo("[TaskUdpReadMsg] Error: client closed connection.\n");
//        if (rdata != NULL)
//            delete rdata;
//        delete line;
//    }
//    else
//    {
//        size = n;
//        for (i = 0; i < n; ++i)
//        {
//            if (line[i] == '\n')
//            {
//                line[i] = '\0';
//                size = i + 1;
//            }
//        }
//        echo("[TaskUdpReadMsg] received %d : [%d byte] %s\n", fd, size, line);
//        if (line[0] != '\0')
//        {
//            if (!rdata->srv->TriggerSend(fd, line, size))
//                echo("[TaskUdpReadMsg] Error: trigger send fail.\n");
//        }
//    }
    TRACE_FUNC_LEAVE
}

