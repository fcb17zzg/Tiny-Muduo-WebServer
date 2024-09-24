#ifndef TINY_MUDUO_ACCEPTOR_H_
#define TINY_MUDUO_ACCEPTOR_H_

#include <functional>

namespace tiny_muduo {

class EventLoop;
class Address;
class Channel;

class Acceptor {
public:
    typedef std::function<void(int)> NewConnectionCallback;

    Acceptor(EventLoop *loop, Address const &address);

    void BindListenFd(Address const &address);
    void Listen();
    void NewConnection();

    void SetNewConnectionCallback(NewConnectionCallback const &callback) {
        new_connection_callback_ = callback;
    }

private:
    EventLoop *loop_;
    int listenfd_;
    Channel *channel_;

    NewConnectionCallback new_connection_callback_;
};

} // namespace tiny_muduo
#endif
