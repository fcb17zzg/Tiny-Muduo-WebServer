#ifndef TINY_MUDUO_HTTPRESPONSE_H_
#define TINY_MUDUO_HTTPRESPONSE_H_

#include "httprequest.h"
#include <string>
#include <utility>

using std::string;

namespace tiny_muduo {

static string const CRLF = "\r\n";

enum HttpStatusCode {
    k100Continue = 100,
    k200OK = 200,
    k400BadRequest = 400,
    k403Forbidden = 403,
    k404NotFound = 404,
    k500InternalServerErrno = 500
};

class Buffer;

class HttpResponse {
public:
    HttpResponse(bool close_connection)
        : type_("text/plain"),
          close_connection_(close_connection) {}

    ~HttpResponse() {}

    void SetStatusCode(HttpStatusCode status_code) {
        status_code_ = status_code;
    }

    void SetStatusMessage(string const &status_message) {
        status_message_ = std::move(status_message);
    }

    void SetCloseConnection(bool close_connection) {
        close_connection_ = close_connection;
    }

    void SetBodyType(string const &type) {
        type_ = type;
    }

    void SetBodyType(char const *type) {
        type_ = type;
    }

    void SetBody(string const &body) {
        body_ = body;
    }

    void SetBody(char const *body) {
        body_ = std::move(string(body));
    }

    void AppendToBuffer(Buffer *buffer);

    bool CloseConnection() {
        return close_connection_;
    }

private:
    static string const server_name_;
    HttpStatusCode status_code_;
    string status_message_;
    string headers_;
    string body_;
    string type_;
    bool close_connection_;
};

} // namespace tiny_muduo

#endif
