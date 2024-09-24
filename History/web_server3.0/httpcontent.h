#ifndef TINY_MUDUO_HTTPCONTENT_H_
#define TINY_MUDUO_HTTPCONTENT_H_

#include "buffer.h"
#include "httpparsestate.h"
#include "httprequest.h"
#include <algorithm>
#include <utility>

namespace tiny_muduo {

enum HttpRequestParseLine {
    kLineOK,
    kLineMore,
    kLineErrno
};

class HttpContent {
public:
    HttpContent();
    ~HttpContent();

    void ParseLine(Buffer *buffer);
    bool ParseContent(Buffer *buffer);

    bool GetCompleteRequest() {
        return parse_state_ == kParseGotCompleteRequest;
    }

    HttpRequest const &request() const {
        return request_;
    }

    void ResetContentState() {
        parse_state_ = kParseRequestLine;
        line_state_ = kLineOK;
    }

private:
    int checked_index_;
    HttpRequest request_;
    HttpRequestParseLine line_state_;
    HttpRequestParseState parse_state_;
};

} // namespace tiny_muduo
#endif
