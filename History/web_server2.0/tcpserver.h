#ifndef TINY_MUDUO_TCP_SERVER_H
#define TINY_MUDUO_TCP_SERVER_H

#include "acceptor.h"
#include "callback.h"
#include "eventloop.h"
#include "eventloopthreadpool.h"

namespace tiny_muduo {
class Address;
class EventLoopThreadPool;

class TcpServer {
public:
    TcpServer(EventLoop *loop, Address const &listenAddr);
    ~TcpServer();

    void start() {
        threads_->StartLoop();
        loop_->RunOneFunc(std::bind(&Acceptor::Listen, acceptor_));
    }

    void SetConnectionCallback(ConnectionCallback const &callback) {
        connectionCallback_ = callback;
    }

    void SetMessageCallback(MessageCallback const &callback) {
        messageCallback_ = callback;
    }

    void SetThreadNums(int thread_nums) {
        threads_->SetThreadNums(thread_nums);
    }

    void NewConnection(int connfd);

private:
    EventLoop *loop_;
    EventLoopThreadPool *threads_;
    Acceptor *acceptor_;

    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
};
} // namespace tiny_muduo
#endif
