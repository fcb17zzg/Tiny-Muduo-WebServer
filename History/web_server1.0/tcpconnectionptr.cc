#include "tcpconnectionptr.h"
#include "channel.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

using namespace tiny_muduo;

TcpConnectionPtr::TcpConnectionPtr(EventLoop *loop, int connfd)
    : loop_(loop),
      connfd_(connfd),
      channel_(new Channel(loop_, connfd_)) {
    channel_->SetReadCallback(
        std::bind(&TcpConnectionPtr::HandleMessage, this));
}

void TcpConnectionPtr::HandleMessage() {
    if (Recv() > 0) {
        message_callback_(this);
    }
}

void TcpConnectionPtr::Send(string const &message) {
    strcpy(buf_, message.c_str());
    send(connfd_, (void const *)(buf_), sizeof(buf_), 0);
}

string TcpConnectionPtr::Get() {
    string message(buf_);
    memset(buf_, '\0', sizeof(buf_));
    return message;
}
