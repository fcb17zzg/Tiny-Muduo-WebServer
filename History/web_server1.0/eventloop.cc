#include "eventloop.h"
#include "channel.h"
#include <cstdio>

namespace tiny_muduo {
EventLoop::EventLoop() : epoller_(new Epoller()), wakeup_fd_(0) {}

EventLoop::~EventLoop() {}

void EventLoop::Loop() {
    while (1) {
        epoller_->Poll(active_channels_);
        printf("EventLoop Loop:: eventnum %d\n", active_channels_.size());
        for (auto const &channel: active_channels_) {
            printf("EventLoop Loop connfd %d\n", channel->fd());
            channel->HandleEvent();
        }
        active_channels_.clear();
    }
}

} // namespace tiny_muduo
