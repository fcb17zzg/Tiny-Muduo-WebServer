#ifndef TINY_MUDUO_ECHO_H
#define TINY_MUDUO_ECHO_H

#include "tcpconnectionptr.h"
#include "tcpserver.h"
#include <stdio.h>
#include <string>

static int const thread_nums = 8;

namespace tiny_muduo {
class Address;
class EventLoop;
} // namespace tiny_muduo

class EchoServer {
public:
    EchoServer(tiny_muduo::EventLoop *loop,
               tiny_muduo::Address const &listen_addr);

    ~EchoServer() {}

    void Start() {
        server_.start();
    }

    void ConnectionCallback(tiny_muduo::TcpConnectionPtr *connection_ptr) {
        printf("echo_server has a new connection \n");
    }

    void MessageCallback(tiny_muduo::TcpConnectionPtr *connection_ptr) {
        std::string message(connection_ptr->Get());
        printf("echo_server get message \n");
        connection_ptr->Send(message);
    }

private:
    tiny_muduo::EventLoop *loop_;
    tiny_muduo::TcpServer server_;
};
#endif
