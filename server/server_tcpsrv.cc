#include "server_tcpsrv.h"

TcpServer::TcpServer()
{
    threadPool = new ThreadPool(4);

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

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    const char *local_addr = LOCAL_ADDR;
    inet_aton(local_addr, &(serveraddr.sin_addr));
    serveraddr.sin_port = htons(SERV_PORT);
    bind(listenfd, (sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(listenfd, LISTENQ);
}

TcpServer::~TcpServer()
{
    delete threadPool;
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
                echo("[SERVER] connect from %s \n", inet_ntoa(clientaddr.sin_addr));
                ev.data.fd = connfd;
                // monitor in message, edge trigger
                ev.events = EPOLLIN | EPOLLET;
                // add fd to epoll queue
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
            }
            // Received data
            else if (events[i].events & EPOLLIN)
            {
                if (events[i].data.fd < 0)
                    continue;
                echo("[SERVER] put task %d to read queue\n", events[i].data.fd);
                TcpPkg* pkg= new TcpPkg;
                pkg->fd = events[i].data.fd;
                pkg->srv = this;
                Task *task = TaskFactory::Ins()->CreateTask(TASK_TYPE_READ_MSG, (void*)pkg);
                threadPool->enqueue(task);
            }
            // Have data to send
            else if (events[i].events & EPOLLOUT)
            {
                if (events[i].data.ptr == NULL)
                    continue;
                if (sendingQueue.size() == 0)
                    continue;
                echo("[SERVER] put task %d to write queue\n", events[i].data.fd);
                Task *task = TaskFactory::Ins()->CreateTask(TASK_TYPE_WRITE_MSG, 
                        (void*)sendingQueue.front());
                threadPool->enqueue(task);
            }
            else
            {
                echo("[SERVER] Error: unknown epoll event");
            }
        }
    }
}

bool TcpServer::TriggerSend(int fd, char* msg, int len)
{
    try
    {
        echo("[TcpServer] trigger to send.");
        TcpPkg *pkg = new TcpPkg();
        pkg->fd = fd;
        pkg->size = len;
        pkg->msg = msg;
        // request will be handle in separate thread
        ev.events = EPOLLOUT | EPOLLET;
        // modify moditored fd event
        epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
    }
    catch(...)
    {
        return false;
    }
    return true;
}

void TcpServer::WaitRecv(int fd)
{
    echo("[TcpServer] wait to recv.");
    // Modify monitored event to EPOLLIN, wait next loop to receive data
    ev.events = EPOLLIN | EPOLLET;
    // modify moditored fd event
    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
}
