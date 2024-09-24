#ifndef TINY_MUDUO_CALLBACK_H
#define TINY_MUDUO_CALLBACK_H

#include <functional>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

namespace tiny_muduo {
class TcpConnectionPtr;
typedef std::function<void(TcpConnectionPtr *)> ConnectionCallback;
typedef std::function<void(TcpConnectionPtr *)> MessageCallback;
typedef std::function<void()> ReadCallback;
typedef std::function<void()> WriteCallback;
} // namespace tiny_muduo
#endif // TINY_MUDUO_CALLBACK_H
