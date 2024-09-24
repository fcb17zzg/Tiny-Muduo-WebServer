#include "httpresponse.h"
#include "buffer.h"
#include <stdio.h>
#include <string>

using namespace tiny_muduo;
using std::string;

string const HttpResponse::server_name_ = "Tiny_muduo";

void HttpResponse::AppendToBuffer(Buffer *buffer) {
    char buf[32] = {0};

    snprintf(buf, sizeof(buf), "HTTP/1.1 %d ", status_code_);
    buffer->Append(buf);
    buffer->Append(status_message_);
    buffer->Append(CRLF);

    if (close_connection_) {
        buffer->Append("Connection: close\r\n");
    } else {
        snprintf(
            buf, sizeof(buf), "Content-Length: %zd\r\n",
            body_.size()); // no need to memset this is longer than HTTP... one
        buffer->Append(buf);
        buffer->Append("Connection: Keep-Alive\r\n");
    }

    buffer->Append("Content-Type: ");
    buffer->Append(type_);
    buffer->Append(CRLF);

    buffer->Append("Server: ");
    buffer->Append(server_name_);
    buffer->Append(CRLF);
    buffer->Append(CRLF);

    buffer->Append(body_);
    return;
}
