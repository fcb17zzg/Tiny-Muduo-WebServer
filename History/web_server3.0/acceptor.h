#ifndef TINY_MUDUO_ACCEPTOR_H_
#define TINY_MUDUO_ACCEPTOR_H_

#include <functional>
#include <memory>

namespace tiny_muduo {

class EventLoop;
class Address;
class Channel;

class Acceptor {
public:
    typedef std::function<void(int)> NewConnectionCallback;

    Acceptor(EventLoop *loop, Address const &address);
    ~Acceptor();

    void SetNonBlocking(int fd); // for accept non-blocking not for accept4
    void BindListenFd(Address const &address);
    void Listen();
    void NewConnection();

    void SetNewConnectionCallback(NewConnectionCallback const &callback) {
        new_connection_callback_ = std::move(callback);
    }

private:
    EventLoop *loop_;
    int listenfd_;
    std::unique_ptr<Channel> channel_;

    NewConnectionCallback new_connection_callback_;
};

} // namespace tiny_muduo
#endif
