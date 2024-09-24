#include "eventloop.h"
#include "channel.h"
#include "mutex.h"
#include <cstdio>
#include <pthread.h>
#include <sys/eventfd.h>
#include <unistd.h>

using namespace tiny_muduo;

EventLoop::EventLoop()
    : tid_(::pthread_self()),
      epoller_(new Epoller()),
      wakeup_fd_(::eventfd(0, EFD_NONBLOCK)),
      wakeup_channel_(new Channel(this, wakeup_fd_)) {
    wakeup_channel_->SetReadCallback(std::bind(&EventLoop::HandleRead, this));
    wakeup_channel_->EnableReading();
}

EventLoop::~EventLoop() {}

void EventLoop::Loop() {
    while (1) {
        epoller_->Poll(active_channels_);
        printf("EventLoop Loop:: eventnum %d\n", active_channels_.size());
        for (auto const &channel: active_channels_) {
            channel->HandleEvent();
        }
        active_channels_.clear();
        DoToDoList();
    }
}

void EventLoop::HandleRead() {
    uint64_t read_one_byte = 1;
    ::read(wakeup_fd_, &read_one_byte, sizeof(read_one_byte));
    return;
}

void EventLoop::RunOneFunc(BasicFunc const &func) {
    if (IsInThreadLoop()) {
        func();
    } else {
        {
            MutexLock lock(mutex_);
            to_do_list_.emplace_back(func);
        }
        if (!IsInThreadLoop()) {
            uint64_t write_one_byte = 1;
            ::write(wakeup_fd_, &write_one_byte, sizeof(write_one_byte));
        }
    }
}

void EventLoop::DoToDoList() {
    ToDoList functors;
    {
        MutexLockGuard lock(mutex_);
        functors.swap(to_do_list_);
    }

    for (auto const &func: functors) {
        func();
    }
}
