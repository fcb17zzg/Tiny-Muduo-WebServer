#ifndef TINY_MUDUO_TCPSERVER_H_
#define TINY_MUDUO_TCPSERVER_H_

#include "acceptor.h"
#include "callback.h"
#include "eventloop.h"
#include "eventloopthreadpool.h"
#include "tcpconnection.h"
#include <memory>
#include <unordered_map>

namespace tiny_muduo
{

    class Address;

    class TcpServer
    {
    public:
        TcpServer(EventLoop *loop, Address const &address);
        ~TcpServer();

        void Start()
        {
            threads_->StartLoop();
            loop_->RunOneFunc(std::bind(&Acceptor::Listen, acceptor_.get()));
        }

        void SetConnectionCallback(ConnectionCallback &&callback)
        {
            connection_callback_ = std::move(callback);
        }

        void SetConnectionCallback(const ConnectionCallback &callback)
        {
            connection_callback_ = callback;
        }

        void SetMessageCallback(MessageCallback &&callback)
        {
            message_callback_ = std::move(callback);
        }

        void SetMessageCallback(const MessageCallback &callback)
        {
            message_callback_ = callback;
        }
        void SetThreadNums(int thread_nums)
        {
            threads_->SetThreadNums(thread_nums);
        }

        void HandleClose(TcpConnectionPtr const &conn);
        void HandleCloseInLoop(TcpConnectionPtr const &ptr);
        void HandleNewConnection(int connfd);

    private:
        typedef std::unordered_map<int, TcpconnectionPtr> ConnectionMap;

        EventLoop *loop_;
        std::unique_ptr<EventLoopThreadPool> threads_;
        std::unique_ptr<Acceptor> acceptor_;

        ConnectionCallback connection_callback_;
        MessageCallback message_callback_;
        ConnectionMap connections_;
    };

} // namespace tiny_muduo
#endif
