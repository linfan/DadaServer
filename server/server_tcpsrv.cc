#include "server_tcpsrv.h"
#include "utility.h"

TcpServer::TcpServer(ThreadPool* pool)
{
    TRACE_FUNC_BEGIN
    threadPool = pool;

    // epoll descriptor, for handling accept
    epfd = epoll_create(256);
    listenfd = socket(PF_INET, SOCK_STREAM, 0);
    // set the descriptor as non-blocking
    setnonblocking(listenfd);
    // event related descriptor
    ev.data.fd = listenfd;
    // monitor in message, edge trigger
    ev.events = EPOLLIN | EPOLLET;
    // register epoll event
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

    TRACE_FUNC_LEAVE
}

void TcpServer::Connect(char *host, uint16_t port)
{
    TRACE_FUNC_BEGIN
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_aton(host, &(serveraddr.sin_addr));
    serveraddr.sin_port = htons(port);
    bind(listenfd, (sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(listenfd, LISTENQ);
    TRACE_FUNC_LEAVE
}

TcpServer::~TcpServer()
{
    TRACE_FUNC_BEGIN
    delete threadPool;
    TRACE_FUNC_LEAVE
}

void TcpServer::setnonblocking(int sock)
{
    int opts;
    if ((opts = fcntl(sock, F_GETFL)) < 0)
        errexit("GETFL %d failed", sock);
    opts = opts | O_NONBLOCK;
    if (fcntl(sock, F_SETFL, opts) < 0)
        errexit("SETFL %d failed", sock);
}

void TcpServer::Run()
{
    TRACE_FUNC_BEGIN
    for(;;)
    {
        // waiting for epoll event
        nfds = epoll_wait(epfd, events, LISTENQ, TIMEOUT);

        // In case of edge trigger, must go over each event
        for (i = 0; i < nfds; ++i)
        {
            // Get new connection
            if (events[i].data.fd == listenfd)
            {
                // accept the client connection
                connfd = accept(listenfd, (sockaddr*)&clientaddr, &clilen);
                if (connfd < 0)
                    errexit("connfd < 0");
                setnonblocking(connfd);
                echo("[TcpServer] connect from %s \n", inet_ntoa(clientaddr.sin_addr));
                ev.data.fd = connfd;
                // monitor in message, edge trigger
                ev.events = EPOLLIN | EPOLLET;
                // add fd to epoll queue
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
            }
            // Received data
            else if (events[i].events & EPOLLIN)
            {
                echo("[TcpServer] got a read request from %d \n", events[i].data.fd);
                if (events[i].data.fd < 0)
                    continue;
                echo("[TcpServer] put task %d to read queue\n", events[i].data.fd);
                TcpPkg* pkg= new TcpPkg;
                pkg->fd = events[i].data.fd;
                pkg->srv = this;
                Task *task = TaskFactory::Ins()->CreateTask(TASK_TYPE_TCP_READ_MSG, (void*)pkg);
                threadPool->enqueue(task);
            }
            // Have data to send
            else if (events[i].events & EPOLLOUT)
            {
                echo("[TcpServer] got a write request from %d \n", events[i].data.fd);
                if (events[i].data.ptr == NULL)
                    continue;
                if (sendingQueue.size() == 0)
                    continue;
                echo("[TcpServer] put task %d to write queue\n", events[i].data.fd);
                Task *task = TaskFactory::Ins()->CreateTask(TASK_TYPE_TCP_WRITE_MSG, 
                        (void*)sendingQueue.front());
                sendingQueue.pop();
                threadPool->enqueue(task);
            }
            else
            {
                echo("[TcpServer] Error: unknown epoll event\n");
            }
        }
    }
    TRACE_FUNC_LEAVE
}

bool TcpServer::TriggerSend(int fd, char* msg, int len)
{
    TRACE_FUNC_BEGIN
    try
    {
        TcpPkg *pkg = new TcpPkg();
        pkg->fd = fd;
        pkg->size = len;
        pkg->msg = new string(msg);
        pkg->srv = this;
        sendingQueue.push(pkg);
        // request will be handle in separate thread
        ContinueSend(fd);
    }
    catch(...)
    {
        TRACE_FUNC_RET_D(0)
        return false;
    }
    TRACE_FUNC_RET_D(1)
    return true;
}

void TcpServer::ContinueSend(int fd)
{
    TRACE_FUNC_BEGIN
    // Modify monitored event to EPOLLOUT, wait next loop to send data
    ev.events = EPOLLOUT | EPOLLET;
    // modify moditored fd event
    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
    TRACE_FUNC_LEAVE
}

void TcpServer::ContinueRecv(int fd)
{
    TRACE_FUNC_BEGIN
    echo("[TcpServer] continue to recv.\n");
    // Modify monitored event to EPOLLIN, wait next loop to receive data
    ev.events = EPOLLIN | EPOLLET;
    // modify moditored fd event
    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
    TRACE_FUNC_LEAVE
}

