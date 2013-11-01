#include <sys/socket.h>      // recv() and send()
#include <sys/epoll.h>       // epoll interface
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>          // strerror()
#include <errno.h>

#include "task_writemsg.h"
#include "server_tcpsrv.h"
#include "utility.h"

void TaskWriteMsg::run()
{
    unsigned int n;
    // data to wirte back to client
    TcpPkg *rdata = (TcpPkg*)arg;

    echo("[TaskWriteMsg] sending %d : [%d byte] %s\n", rdata->fd, rdata->size, rdata->msg->c_str());
    // send responce to client
    if ((n = send(rdata->fd, rdata->msg->c_str(), rdata->size, 0)) < 0)
    {
        if (errno == ECONNRESET)
            close(rdata->fd);
        echo("[TaskWriteMsg] Error: send responce failed: %s\n", strerror(errno));
    }
    else if (n == 0)
    {
        close(rdata->fd);
        echo("[TaskWriteMsg] Error: client closed connection.\n");
    }
    else
    {
        echo("[TaskWriteMsg] Succeed: message send to client.\n");
        rdata->srv->ContinueRecv(rdata->fd);
    }
    // delete data
    delete rdata->msg;
    delete rdata;
}
