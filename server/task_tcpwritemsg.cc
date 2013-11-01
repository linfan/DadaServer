#include <sys/socket.h>      // recv() and send()
#include <sys/epoll.h>       // epoll interface
#include <unistd.h>
#include <stdio.h>
#include <string.h>          // strerror()
#include <errno.h>

#include "server_tcpsrv.h"
#include "task_tcpwritemsg.h"
#include "utility.h"

void TaskTcpWriteMsg::run()
{
    TRACE_FUNC_BEGIN
    unsigned int n;
    // data to wirte back to client
    TcpPkg *rdata = (TcpPkg*)arg;

    echo("[TaskTcpWriteMsg] sending %d : [%d byte] %s\n", rdata->fd, rdata->size, rdata->msg->c_str());
    // send responce to client
    if ((n = send(rdata->fd, rdata->msg->c_str(), rdata->size, 0)) < 0)
    {
        if (errno == ECONNRESET)
            close(rdata->fd);
        echo("[TaskTcpWriteMsg] Error: send responce failed: %s\n", strerror(errno));
    }
    else if (n == 0)
    {
        close(rdata->fd);
        echo("[TaskTcpWriteMsg] Error: client closed connection.\n");
    }
    else
    {
        echo("[TaskTcpWriteMsg] Succeed: message send to client.\n");
        rdata->srv->ContinueRecv(rdata->fd);
    }
    // delete data
    delete rdata->msg;
    delete rdata;
    TRACE_FUNC_LEAVE
}
