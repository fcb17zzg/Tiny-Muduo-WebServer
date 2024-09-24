#ifndef TINY_MUDUO_CHANNEL_H_
#define TINY_MUDUO_CHANNEL_H_

#include "callback.h"
#include "eventloop.h"
#include <sys/epoll.h>
#include <utility>

namespace tiny_muduo {

enum ChannelState {
    kNew,
    kAdded,
    kDeleted
};

class Channel {
public:
    Channel(EventLoop *loop, int const &fd);
    ~Channel();

    void HandleEvent();

    void SetReadCallback(ReadCallback callback) {
        read_callback_ = std::move(callback);
    }

    void SetWriteCallback(WriteCallback callback) {
        write_callback_ = std::move(callback);
    }

    void EnableReading() {
        events_ |= (EPOLLIN | EPOLLPRI);
        Update();
    }

    void EnableWriting() {
        events_ |= EPOLLOUT;
        Update();
    }

    void DisableAll() {
        events_ = 0;
        Update();
    }

    void DisableWriting() {
        events_ &= ~EPOLLOUT;
        Update();
    }

    void Update() {
        loop_->Update(this);
    }

    void SetReceivedEvents(int events) {
        recv_events_ = events;
    }

    void SetChannelState(ChannelState state) {
        state_ = state;
    }

    int fd() const {
        return fd_;
    }

    int events() const {
        return events_;
    }

    int recv_events() const {
        return recv_events_;
    }

    ChannelState state() const {
        return state_;
    }

    bool IsWriting() {
        return events_ & EPOLLOUT;
    }

    bool IsReading() {
        return events_ & (EPOLLIN | EPOLLPRI);
    }

private:
    EventLoop *loop_;
    int fd_;
    int events_;      // update events
    int recv_events_; // epoll received events

    ChannelState state_;
    ReadCallback read_callback_;
    WriteCallback write_callback_;
};

} // namespace tiny_muduo
#endif
