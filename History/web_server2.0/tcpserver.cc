#include "tcpserver.h"
#include "acceptor.h"
#include "eventloopthreadpool.h"
#include "tcpconnectionptr.h"

using namespace tiny_muduo;

TcpServer::TcpServer(EventLoop *loop, Address const &address)
    : loop_(loop),
      threads_(nullptr),
      acceptor_(new Acceptor(loop, address)) {
    acceptor_->SetNewConnectionCallback(
        std::bind(&TcpServer::NewConnection, this, _1));
}

TcpServer::~TcpServer() {
    delete acceptor_;
    delete threads_;
}

void TcpServer::NewConnection(int connfd) {
    EventLoop *loop = threads_->NextLoop();
    printf("TcpServer NewConnection Arrive Tid:%ld Manage\n",
           loop->DebugShowTid());
    TcpConnectionPtr *ptr = new TcpConnectionPtr(loop_, connfd);
    ptr->SetConnectionCallback(connectionCallback_);
    ptr->SetMessageCallback(messageCallback_);
    loop_->RunOneFunc(std::bind(&TcpConnectionPtr::ConnectionEstablished, ptr));
}
