#ifndef TINY_MUDUO_EVENTLOOP_H
#define TINY_MUDUO_EVENTLOOP_H

#include "epoller.h"
#include <functional>
#include <vector>

namespace tiny_muduo {

class Epoller;
class Channel;

class EventLoop {
public:
    typedef std::vector<Channel *> Channels;
    typedef std::function<void()> BasicFunc;

    EventLoop();
    ~EventLoop();

    void Loop();

    void Update(Channel *channel) {
        epoller_->Update(channel);
    }

    void RunOneFunc(BasicFunc const &func) {
        func();
    }

private:
    Epoller *epoller_;
    int wakeup_fd_;
    Channels active_channels_;
};
} // namespace tiny_muduo

#endif
