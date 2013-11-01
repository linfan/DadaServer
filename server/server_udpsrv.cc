#include "server_udpsrv.h"
#include "utility.h"

UdpServer::UdpServer(ThreadPool* pool)
{
    TRACE_FUNC_BEGIN
    threadPool = new ThreadPool(4);
    TRACE_FUNC_LEAVE
}

UdpServer::~UdpServer()
{
    TRACE_FUNC_BEGIN
    delete threadPool;
    TRACE_FUNC_LEAVE
}

void UdpServer::Connect(char *host, uint16_t port)
{
    TRACE_FUNC_BEGIN

    memset(&sin, 0, sizeof(sin));
    // TCP/IP suite
    sin.sin_family = AF_INET;
    // Use any local IP, need translate to internet byte order
    if (host)
        inet_aton(host, &(serveraddr.sin_addr));
    else
        sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);
    // Create socket
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        errexit("can't create socket: %s \n", strerror(errno));
    // Bind socket to service-end address
    if (bind(sock, (struct sockaddr*)&sin, sizeof(sin)) < 0)
        errexit("can't bind to %s port: %s \n", service, strerror(errno));

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

