#ifndef TINY_MUDUO_CALLBACK_H_
#define TINY_MUDUO_CALLBACK_H_

#include <functional>
#include <memory>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

namespace tiny_muduo {
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

class Buffer;
typedef std::function<void(TcpConnectionPtr const &, Buffer *)>
    ConnectionCallback;
typedef std::function<void(TcpConnectionPtr const &, Buffer *)> MessageCallback;
typedef std::function<void()> ReadCallback;
typedef std::function<void()> WriteCallback;
typedef std::function<void(TcpConnectionPtr const &)> CloseCallback;
} // namespace tiny_muduo
#endif
