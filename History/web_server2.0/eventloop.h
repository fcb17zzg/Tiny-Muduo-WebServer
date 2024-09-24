#ifndef TINY_MUDUO_EVENTLOOP_H
#define TINY_MUDUO_EVENTLOOP_H

#include "epoller.h"
#include "mutex.h"
#include <functional>
#include <pthread.h>
#include <stdint.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <vector>

namespace tiny_muduo {

class Epoller;
class Channel;

class EventLoop {
public:
    typedef std::vector<Channel *> Channels;
    typedef std::function<void()> BasicFunc;
    typedef std::vector<BasicFunc> ToDoList;

    EventLoop();
    ~EventLoop();

    bool IsInThreadLoop() {
        return ::pthread_self() == tid_;
    }

    void Loop();
    void HandleRead();
    void DoToDoList();

    void Update(Channel *channel) {
        epoller_->Update(channel);
    }

    pthread_t DebugShowTid() {
        return tid_;
    }

    void RunOneFunc(BasicFunc const &func);

private:
    pthread_t tid_;
    Epoller *epoller_;
    int wakeup_fd_;
    Channel *wakeup_channel_;
    Channels active_channels_;
    ToDoList to_do_list_;

    MutexLock mutex_;
};
} // namespace tiny_muduo

#endif
