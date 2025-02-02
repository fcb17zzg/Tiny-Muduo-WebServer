#ifndef TINY_MUDUO_EPOLLER_H
#define TINY_MUDUO_EPOLLER_H

#include <cstdio>
#include <sys/epoll.h>
#include <vector>

namespace {
int const kMaxEvents = 8;
}

namespace tiny_muduo {
class Channel;

class Epoller {
public:
    typedef std::vector<epoll_event> Events;
    typedef std::vector<Channel *> Channels;
    Epoller();

    void Poll(Channels &channels);

    int EpollWait() {
        return epoll_wait(epollfd_, &*events_.begin(), kMaxEvents, -1);
    }

    int SetNonBlocking(int fd);
    void FillActiveChannels(int eventnums, Channels &channels);
    void Update(Channel *channel);
    void UpdateChannel(int operation, Channel *channel);

private:
    int epollfd_;
    Events events_;
};
} // namespace tiny_muduo
#endif
