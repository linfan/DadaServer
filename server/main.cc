#include "server_tcpsrv.h"
#include "server_udpsrv.h"
#include "utility.h"

#define LOCAL_ADDR  "127.0.0.1"
#define SERV_PORT   10086

int main(int argc,char* argv[])
{
    TRACE_MAIN_BEGIN

    ThreadPool *threadPool = new ThreadPool(4);

    TcpServer server(threadPool);
    server.Connect(LOCAL_ADDR, SERV_PORT);
    server.Run();

    TRACE_MAIN_LEAVE
    return 0;
}


