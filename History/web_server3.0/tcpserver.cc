#include "tcpserver.h"

#define NDEBUG
#include "acceptor.h"
#include "eventloopthreadpool.h"
#include "tcpconnection.h"
#include <assert.h>
#include <utility>

using namespace tiny_muduo;

TcpServer::TcpServer(EventLoop *loop, Address const &address)
    : loop_(loop),
      threads_(new EventLoopThreadPool(loop_)),
      acceptor_(new Acceptor(loop_, address)) {
    acceptor_->SetNewConnectionCallback(
        std::bind(&TcpServer::HandleNewConnection, this, _1));
}

TcpServer::~TcpServer() {
    for (auto &pair: connections_) {
        TcpConnectionPtr ptr(pair.second);
        pair.second.reset();
        ptr->loop()->RunOneFunc(
            std::bind(&TcpConnection::ConnectionDestructor, ptr));
    }
}

void TcpServer::HandleClose(TcpConnectionPtr const &ptr) {
    loop_->QueueOneFunc(std::bind(&TcpServer::HandleCloseInLoop, this, ptr));
}

void TcpServer::HandleCloseInLoop(TcpConnectionPtr const &ptr) {
    assert(connections_.find(ptr->fd()) != connections_.end());
    connections_.erase(connections_.find(ptr->fd()));
    EventLoop *loop = ptr->loop();
    loop->QueueOneFunc(std::bind(&TcpConnection::ConnectionDestructor, ptr));
}

void TcpServer::HandleNewConnection(int connfd) {
    EventLoop *loop = threads_->NextLoop();
    TcpConnectionPtr ptr(new TcpConnection(loop, connfd));
    connections_[connfd] = ptr;
    ptr->SetConnectionCallback(connection_callback_);
    ptr->SetMessageCallback(message_callback_);
    ptr->SetCloseCallback(std::bind(&TcpServer::HandleClose, this, _1));
    loop->RunOneFunc(std::bind(&TcpConnection::ConnectionEstablished, ptr));
}
