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

    echo("[SERVER] writetask %d sending %d : [%d] %s\n", pthread_self(), rdata->fd, rdata->size, rdata->msg);
    // send responce to client
    if ((n = send(rdata->fd, rdata->msg, rdata->size, 0)) < 0)
    {
        if (errno == ECONNRESET)
            close(rdata->fd);
        echo("[SERVER] Error: send responce failed: %s\n", strerror(errno));
    }
    else if (n == 0)
    {
        close(rdata->fd);
        echo("[SERVER] Error: client closed connection.");
    }
    else
    {
        echo("[SERVER] Succeed: message send to client.");
        rdata->srv->WaitRecv(rdata->fd);
    }
    // delete data
    delete rdata;
}
