#include "channel.h"
#include <sys/epoll.h>

using namespace tiny_muduo;

Channel::Channel(EventLoop *loop, int const &fd)
    : loop_(loop),
      fd_(fd),
      events_(0),
      recv_events_(0) {}

void Channel::HandleEvent() {
    if (recv_events_ & EPOLLIN) {
        read_callback_();
    } else if (recv_events_ & EPOLLOUT) {
        write_callback_();
    } else {
    }
}
