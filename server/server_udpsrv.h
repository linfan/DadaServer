#ifndef _SERVER_UDP_SERVER_H_
#define _SERVER_UDP_SERVER_H_

#include <sys/socket.h>      // socket interface
#include <netinet/in.h>      // struct sockaddr_in
#include <arpa/inet.h>       // IP addr convertion
#include <fcntl.h>           // File descriptor controller
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>          // bzero()
#include <stdlib.h>          // malloc(), delete 
#include <errno.h>
#include <queue>
#include <string>

#include "utility.h"
#include "threadpool.h"
#include "threadworker.h"
#include "task_factory.h"
#include "task_udpreadmsg.h"
#include "task_udpwritemsg.h"
#include "server.h"

#define OPEN_MAX    100
#define LISTENQ     20
#define INFTIM      1000
#define TIMEOUT     500

class UdpServer;

// for data transporting
// Don't share it with threads, it's not thread safe
struct UdpPkg
{
    // host and port to UDP connection
    in_addr_t ip;
    uint16_t port;
    // real message length
    int size;
    // message to send
    string *msg;
    // UdpServer
    UdpServer *srv;
};

class UdpServer : public Server
{
    private:
        // message to send
        queue<UdpPkg*> sendingQueue;

        // thread pool
        ThreadPool *threadPool;
        // socket struct
        sockaddr_in serveraddr;

        // Serice-end socket address
        struct sockaddr_in fsin;
        // Service-end socket descriptor
        int sock;
        // Service-end socket
        struct sockaddr_in sin;

    public:
        UdpServer(ThreadPool*);
        virtual ~UdpServer();
        void Connect(char *host, uint16_t port);
        void Run();
};

#endif
