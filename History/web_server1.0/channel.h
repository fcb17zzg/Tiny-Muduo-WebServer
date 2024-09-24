#ifndef TINY_MUDUO_CHANNEL_H
#define TINY_MUDUO_CHANNEL_H

#include "callback.h"
#include "eventloop.h"
#include <sys/epoll.h>

namespace tiny_muduo {

class Channel {
public:
    Channel(EventLoop *loop, int const &fd);
    ~Channel();

    void HandleEvent();

    void SetReadCallback(ReadCallback const &callback) {
        read_callback_ = callback;
    }

    void SetWriteCallback(WriteCallback const &callback) {
        write_callback_ = callback;
    }

    void EnableReading() {
        printf("enable reading\n");
        events_ |= EPOLLIN;
        Update();
    }

    void EnableWriting() {
        events_ |= EPOLLOUT;
        Update();
    }

    void Update() {
        loop_->Update(this);
    }

    void SetReceivedEvents(int events) {
        recv_events_ = events;
    }

    int fd() {
        return fd_;
    }

    int events() {
        return events_;
    }

    int recv_events() {
        return recv_events_;
    }

private:
    EventLoop *loop_;
    int fd_;
    int events_;
    int recv_events_;

    ReadCallback read_callback_;
    WriteCallback write_callback_;
};
} // namespace tiny_muduo

#endif // TINY_MUDUO_CHANNEL_H
