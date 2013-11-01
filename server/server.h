#ifndef _SERVER_H_
#define _SERVER_H_

class Server
{
    public:
        virtual ~Server() { }
        virtual void Connect(char *host, uint16_t port) = 0;
        virtual void Run() = 0;
};

#endif

