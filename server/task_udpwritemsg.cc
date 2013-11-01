#include <sys/socket.h>      // recv() and send()
#include <unistd.h>
#include <stdio.h>
#include <string.h>          // strerror()
#include <errno.h>

#include "server_tcpsrv.h"
#include "task_udpwritemsg.h"
#include "utility.h"

void TaskUdpWriteMsg::run()
{
    TRACE_FUNC_BEGIN
//    unsigned int n;
//    // data to wirte back to client
//    TcpPkg *rdata = (TcpPkg*)arg;
//
//    echo("[TaskUdpWriteMsg] sending %d : [%d byte] %s\n", rdata->fd, rdata->size, rdata->msg->c_str());
//    // send responce to client
//    if ((n = send(rdata->fd, rdata->msg->c_str(), rdata->size, 0)) < 0)
//    {
//        if (errno == ECONNRESET)
//            close(rdata->fd);
//        echo("[TaskUdpWriteMsg] Error: send responce failed: %s\n", strerror(errno));
//    }
//    else if (n == 0)
//    {
//        close(rdata->fd);
//        echo("[TaskUdpWriteMsg] Error: client closed connection.\n");
//    }
//    else
//    {
//        echo("[TaskUdpWriteMsg] Succeed: message send to client.\n");
//        rdata->srv->ContinueRecv(rdata->fd);
//    }
//    // delete data
//    delete rdata->msg;
//    delete rdata;
    TRACE_FUNC_LEAVE
}
