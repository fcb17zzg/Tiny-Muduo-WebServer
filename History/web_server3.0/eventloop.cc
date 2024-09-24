#include "eventloop.h"
#include "channel.h"
#include "mutex.h"
#include <pthread.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <utility>

using namespace tiny_muduo;

EventLoop::EventLoop()
    : tid_(::pthread_self()),
      epoller_(new Epoller()),
      wakeup_fd_(::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC)),
      wakeup_channel_(new Channel(this, wakeup_fd_)),
      calling_functors_(false) {
    wakeup_channel_->SetReadCallback(std::bind(&EventLoop::HandleRead, this));
    wakeup_channel_->EnableReading();
}

EventLoop::~EventLoop() {
    wakeup_channel_->DisableAll();
    Remove(wakeup_channel_.get());
}

void EventLoop::Loop() {
    while (1) {
        active_channels_.clear();
        epoller_->Poll(active_channels_);
        for (auto const &channel: active_channels_) {
            channel->HandleEvent();
        }
        DoToDoList();
    }
}

void EventLoop::HandleRead() {
    uint64_t read_one_byte = 1;
    ::read(wakeup_fd_, &read_one_byte, sizeof(read_one_byte));
    return;
}

void EventLoop::QueueOneFunc(BasicFunc func) {
    {
        MutexLockGuard lock(mutex_);
        to_do_list_.emplace_back(std::move(func));
    }

    if (!IsInThreadLoop() || calling_functors_) {
        uint64_t write_one_byte = 1;
        ::write(wakeup_fd_, &write_one_byte, sizeof(write_one_byte));
    }
}

void EventLoop::RunOneFunc(BasicFunc func) {
    if (IsInThreadLoop()) {
        func();
    } else {
        QueueOneFunc(std::move(func));
    }
}

void EventLoop::DoToDoList() {
    ToDoList functors;
    calling_functors_ = true;
    {
        MutexLockGuard lock(mutex_);
        functors.swap(to_do_list_);
    }

    for (auto const &func: functors) {
        func();
    }
    calling_functors_ = false;
}
