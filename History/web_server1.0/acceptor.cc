#include "acceptor.h"
#include "address.h"
#include "channel.h"
#include <arpa/inet.h>
#include <assert.h>
#include <bits/socket.h>
#include <cstring>
#include <functional>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

using namespace tiny_muduo;

namespace {
int const kMaxListenNum = 5;
}

Acceptor::Acceptor(EventLoop *loop, Address const &address)
    : loop_(loop),
      listenfd_(socket(PF_INET, SOCK_STREAM, 0)),
      channel_(new Channel(loop, listenfd_)) {
    BindListenFd(address);
    channel_->SetReadCallback(std::bind(&Acceptor::NewConnection, this));
}

void Acceptor::BindListenFd(Address const &addr) {
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(addr.port());
    inet_pton(AF_INET, addr.ip(), &address.sin_addr);
    int ret = bind(listenfd_, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);
}

void Acceptor::Listen() {
    int ret = listen(listenfd_, kMaxListenNum);
    assert(ret != -1);
    printf("listenfd_:%d\n", listenfd_);
    channel_->EnableReading();
}

void Acceptor::NewConnection() {
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd =
        accept(listenfd_, (struct sockaddr *)&client, &client_addrlength);
    new_connection_callback_(connfd);
}
