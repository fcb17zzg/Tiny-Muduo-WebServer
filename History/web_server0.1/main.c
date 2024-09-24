#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc <= 2) {
        printf("usage: %s ipaddress port\n", argv[0]);
        return 0;
    }

    char const *ip = argv[1];
    int port = atoi(argv[2]);
    // 创建套接字
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 1);
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &address.sin_addr);

    int ret = 0;
    ret = bind(listenfd, (struct sockaddr *)(&address), sizeof(address));
    assert(ret != -1);

    ret = listen(listenfd, 5);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int sockfd =
        accept(listenfd, (struct sockaddr *)(&address), &client_addrlength);

    char buf_size[1024] = {0};
    int recv_size = 0;
    recv_size = recv(sockfd, buf_size, sizeof(buf_size), 0);

    int send_size = 0;
    recv_size = send(sockfd, buf_size, sizeof(buf_size), 0);
    close(sockfd);
    close(listenfd);
    return 0;
}
