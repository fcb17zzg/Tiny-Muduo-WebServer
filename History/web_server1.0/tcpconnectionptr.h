#ifndef TINY_MUDUO_TCPCONNECTIONPTR_H
#define TINY_MUDUO_TCPCONNECTIONPTR_H

#include "callback.h"
#include "channel.h"
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

using std::string;

namespace tiny_muduo {
class EventLoop;

class TcpConnectionPtr {
public:
    TcpConnectionPtr(EventLoop *loop, int connfd);
    ~TcpConnectionPtr();

    void SetConnectionCallback(ConnectionCallback const &callback) {
        connection_callback_ = callback;
    }

    void SetMessageCallback(MessageCallback const &callback) {
        message_callback_ = callback;
    }

    void ConnectionEstablished() {
        channel_->EnableReading();
        connection_callback_(this);
    }

    void HandleMessage();
    void Send(string const &str);
    string Get();

private:
    int Recv() {
        memset(buf_, '\0', sizeof(buf_));
        int ret = recv(connfd_, buf_, 100, 0);
        return ret;
    }

    EventLoop *loop_;
    int connfd_;
    Channel *channel_;
    char buf_[100] = {0};
    ConnectionCallback connection_callback_;
    MessageCallback message_callback_;
};
} // namespace tiny_muduo

#endif // TINY_MUDUO_TCPCONNECTIONPTR_H
