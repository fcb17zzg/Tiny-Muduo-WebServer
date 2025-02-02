#include "httpserver.h"
#include <functional>

using namespace tiny_muduo;

using tiny_muduo::Version;

HttpServer::HttpServer(EventLoop *loop, Address const &address)
    : loop_(loop),
      server_(loop, address)
{
    server_.SetConnectionCallback(
        std::bind(&HttpServer::ConnectionCallback, this, _1));
    server_.SetMessageCallback(
        std::bind(&HttpServer::MessageCallback, this, _1, _2));
    server_.SetThreadNums(kThreadNums);
    SetHttpResponseCallback(
        std::bind(&HttpServer::HttpDefaultCallback, this, _1, _2));
}

HttpServer::~HttpServer() {}

void HttpServer::MessageCallback(TcpConnection *connection, Buffer *buffer)
{
    if (connection->IsShutdown())
    {
        return;
    }

    HttpContent *content = connection->GetHttpContent();
    if (!content->ParseContent(buffer))
    {
        connection->Send("HTTP/1.1 400 Bad Request\r\n\r\n");
        connection->Shutdown();
        return;
    }

    if (content->GetCompleteRequest())
    {
        DealWithRequest(content->request(), connection);
        content->ResetContentState();
    }
}

void HttpServer::DealWithRequest(HttpRequest const &request,
                                 TcpConnection *connection)
{
    string connection_state = std::move(request.GetHeader("Connection"));
    bool close =
        (connection_state == "Close" ||
         (request.version() == kHttp10 && connection_state != "Keep-Alive"));

    HttpResponse response(close);
    response_callback_(request, response);
    Buffer buffer;
    response.AppendToBuffer(&buffer);
    connection->Send(&buffer);

    if (response.CloseConnection())
    {
        connection->Shutdown();
    }
}
