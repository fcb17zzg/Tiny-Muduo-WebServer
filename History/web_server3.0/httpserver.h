#ifndef TINY_MUDUO_HTTPSERVER_H_
#define TINY_MUDUO_HTTPSERVER_H_

#include "buffer.h"
#include "httpcontent.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "tcpconnection.h"
#include "tcpserver.h"
#include <functional>
#include <stdio.h>
#include <utility>

using tiny_muduo::HttpStatusCode;

namespace tiny_muduo {

static int const kThreadNums = 8;

class EventLoop;

class HttpServer {
    typedef std::function<void(HttpRequest const &, HttpResponse &)>
        HttpResponseCallback;

public:
    HttpServer(EventLoop *loop, Address const &address);
    ~HttpServer();

    void Start() {
        server_.Start();
    }

    void HttpDefaultCallback(HttpRequest const &request,
                             HttpResponse &response) {
        response.SetStatusCode(k404NotFound);
        response.SetStatusMessage("Not Found");
        response.SetCloseConnection(true);
    }

    void ConnectionCallback(TcpConnection *connection) {
        printf("HttpServer has New Conneciton");
    }

    void MessageCallback(TcpConnection *connection, Buffer *buffer);

    void
    SetHttpResponseCallback(HttpResponseCallback const &response_callback) {
        response_callback_ = std::move(response_callback);
    }

    void DealWithRequest(HttpRequest const &request, TcpConnection *connection);

private:
    EventLoop *loop_;
    TcpServer server_;

    HttpResponseCallback response_callback_;
};

} // namespace tiny_muduo

#endif
