#ifndef __ECHO_H_
#define __ECHO_H_

#include "processpool.h"
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class echo {
private:
    static int const BUFFER_SIZE = 1024;
    static int epollfd;
    int sockfd;
    sockaddr_in client_addr;
    char buf[1024] = {0};

public:
    echo() {}

    ~echo() {}

    void init(int _epollfd, int _sockfd, sockaddr_in const &address) {
        epollfd = _epollfd;
        sockfd = _sockfd;
        client_addr = address;
    }

    void process() {
        while (1) {
            memset(buf, 0, sizeof(buf));
            int ret = recv(sockfd, buf, sizeof(buf), 0);
            if (ret < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    break;
                }
            } else if (ret == 0) {
                removefd(epollfd, sockfd);
                close(sockfd);
            } else {
                send(sockfd, buf, sizeof(buf), 0);
            }
        }
        return;
    }
};

int echo::epollfd = -1;
#endif
