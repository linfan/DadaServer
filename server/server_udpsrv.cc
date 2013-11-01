#include "server_udpsrv.h"
#include "utility.h"

UdpServer::UdpServer(ThreadPool* pool)
{
    TRACE_FUNC_BEGIN
    threadPool = new ThreadPool(4);
    TRACE_FUNC_LEAVE
}

void UdpServer::Connect(char *host, uint16_t port)
{
    TRACE_FUNC_BEGIN
//    bzero(&serveraddr, sizeof(serveraddr));
//    serveraddr.sin_family = AF_INET;
//    inet_aton(host, &(serveraddr.sin_addr));
//    serveraddr.sin_port = htons(port);
//    bind(listenfd, (sockaddr*)&serveraddr, sizeof(serveraddr));
//    listen(listenfd, LISTENQ);
    TRACE_FUNC_LEAVE
}

UdpServer::~UdpServer()
{
    TRACE_FUNC_BEGIN
    delete threadPool;
    TRACE_FUNC_LEAVE
}

void UdpServer::Run()
{
    TRACE_FUNC_BEGIN
    for(;;)
    {
    }
    TRACE_FUNC_LEAVE
}

