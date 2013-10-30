#include <sys/socket.h>      // socket interface
#include <sys/epoll.h>       // epoll interface
#include <netinet/in.h>      // struct sockaddr_in
#include <arpa/inet.h>       // IP addr convertion
#include <fcntl.h>           // File descriptor controller
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>          // bzero()
#include <stdlib.h>          // malloc(), delete 
#include <errno.h>
#include "utility.h"
#include "threadpool.h"
#include "threadworker.h"
extern int errno;

#define MAXBTYE     10       // maximum received data byte
#define OPEN_MAX    100
#define LISTENQ     20
#define SERV_PORT   10086
#define INFTIM      1000
#define LOCAL_ADDR  "192.168.1.1"  // "127.0.0.1"
#define TIMEOUT     500

// task item in thread pool
struct Task
{
    // file descriptor or user_data
    epoll_data_t data;
    // next task
    Task* next;
};

// for data transporting
struct UserData
{
    int fd;
    // real received data size
    unsigned int n_size;
    // content received
    char line[MAXBTYE];
};

void *readtask(void *args);
void *writetask(void *args);

// epoll descriptor from epoll_create()
int epfd;
// register epoll_ctl()
epoll_event ev;
// store queued events from epoll_wait()
epoll_event events[LISTENQ];
// mutex lock to protect readhead/readtail
pthread_mutex_t r_mutex;
pthread_cond_t r_condl;
// mutex lock to protect writehead/writetail
pthread_mutex_t w_mutex;
pthread_cond_t w_condl;
// read task list, write task list
Task *readhead = NULL, *readtail = NULL;
Task *writehead = NULL, *writetail = NULL;

void setnonblocking(int sock)
{
    int opts;
    if ((opts = fcntl(sock, F_GETFL)) < 0)
        errexit("GETFL %d failed", sock);
    opts = opts | O_NONBLOCK;
    if (fcntl(sock, F_SETFL, opts) < 0)
        errexit("SETFL %d failed", sock);
}

int main(int argc,char* argv[])
{
    // nfds is number of events (number of returned fd)
    int i, maxi, nfds;
    int listenfd, connfd;
    // read thread pool
    ThreadPool readPool(3);
    // write thread pool
    ThreadPool writePool(3);
    // task node
    Task *new_task = NULL;
    socklen_t clilen;
    sockaddr_in clientaddr;
    sockaddr_in serveraddr;

    pthread_mutex_init(&r_mutex, NULL);
    pthread_cond_init(&r_condl, NULL);
    pthread_mutex_init(&w_mutex, NULL);
    pthread_cond_init(&w_condl, NULL);

    // threads for reading thread pool
    pthread_create(&tid1, NULL, readtask, NULL);
    pthread_create(&tid2, NULL, readtask, NULL);
    // threads for respond to client
    pthread_create(&tid3, NULL, writetask, NULL);
    pthread_create(&tid4, NULL, writetask, NULL);

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

    maxi = 0;
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
                new_task = new Task;
                new_task->data.fd = events[i].data.fd;
                new_task->next = NULL;

                // protect task queue (readhead/readtail)
                pthread_mutex_lock(&r_mutex);
                // the queue is empty
                if (readhead == NULL)
                {
                    readhead = new_task;
                    readtail = new_task;
                }
                // queue is not empty
                else
                {
                    readtail->next = new_task;
                    readtail = new_task;
                }
                // trigger readtask thread
                pthread_cond_broadcast(&r_condl);
                pthread_mutex_unlock(&r_mutex);
            }
            // Have data to send
            else if (events[i].events & EPOLLOUT)
            {
                if (events[i].data.ptr == NULL)
                    continue;
                echo("[SERVER] put task %d to write queue\n", ((Task*)events[i].data.ptr)->data.fd);
                new_task = new Task;
                new_task->data.ptr = (UserData*)events[i].data.ptr;
                new_task->next = NULL;
                pthread_mutex_lock(&w_mutex);
                // the queue is empty
                if (writehead == NULL)
                {
                    writehead = new_task;
                    writetail = new_task;
                }
                // queue is not empty
                else
                {
                    writetail->next = new_task;
                    writetail = new_task;
                }
                // trigger writetask thread
                pthread_cond_broadcast(&w_condl);
                pthread_mutex_unlock(&w_mutex);
            }
            else
            {
                echo("[SERVER] Error: unknown epoll event");
            }
        }
    }

    return 0;
}

void *readtask(void *args)
{
    int fd = -1;
    int n, i;
    UserData* data = NULL;
    while(1)
    {
        // protect task queue (readhead/readtail)
        pthread_mutex_lock(&r_mutex);
        while(readhead == NULL)
        {
            // if condl false, will unlock mutex
            pthread_cond_wait(&r_condl, &r_mutex);
        }

        fd = readhead->data.fd;
        Task* tmp = readhead;
        readhead = readhead->next;
        delete tmp;

        pthread_mutex_unlock(&r_mutex);

        echo("[SERVER] readtask %d handling %d\n", pthread_self(), fd);
        data = new UserData;
        data->fd = fd;
        if ((n = recv(fd, data->line, MAXBTYE, 0)) < 0)
        {
            if (errno == ECONNRESET)
                close(fd);
            echo("[SERVER] Error: readline failed: %s\n", strerror(errno));
            if (data != NULL)
                delete data;
        }
        else if (n == 0)
        {
            close(fd);
            echo("[SERVER] Error: client closed connection.\n");
            if (data != NULL)
                delete data;
        }
        else
        {
            data->n_size = n;
            for (i = 0; i < n; ++i)
            {
                if (data->line[i] == '\n' || data->line[i] > 128)
                {
                    data->line[i] = '\0';
                    data->n_size = i + 1;
                }
            }
            echo("[SERVER] readtask %d received %d : [%d] %s\n", pthread_self(), fd, data->n_size, data->line);
            if (data->line[0] != '\0')
            {
                ev.data.ptr = data;
                // Modify monitored event to EPOLLOUT,  wait next loop to send respond
                ev.events = EPOLLOUT | EPOLLET;
                // modify moditored fd event
                epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
            }
        }
    }
}

void *writetask(void *args)
{
    unsigned int n;
    // data to wirte back to client
    UserData *rdata = NULL;
    while(1)
    {
        pthread_mutex_lock(&w_mutex);
        while(writehead == NULL)
        {
            // if condl false, will unlock mutex
            pthread_cond_wait(&w_condl, &w_mutex);
        }

        rdata = (UserData*)writehead->data.ptr;
        Task* tmp = writehead;
        writehead = writehead->next;
        delete tmp;

        pthread_mutex_unlock(&w_mutex);
        
        echo("[SERVER] writetask %d sending %d : [%d] %s\n", pthread_self(), rdata->fd, rdata->n_size, rdata->line);
        // send responce to client
        if ((n = send(rdata->fd, rdata->line, rdata->n_size, 0)) < 0)
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
            ev.data.fd = rdata->fd;
            // Modify monitored event to EPOLLIN, wait next loop to receive data
            ev.events = EPOLLIN | EPOLLET;
            // modify moditored fd event
            epoll_ctl(epfd, EPOLL_CTL_MOD, rdata->fd, &ev);
        }
        // delete data
        delete rdata;
    }
}

