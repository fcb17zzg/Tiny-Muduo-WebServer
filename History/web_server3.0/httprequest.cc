#include "httprequest.h"
#include "httpparsestate.h"
#include <algorithm>
#include <utility>

using namespace tiny_muduo;
using tiny_muduo::HttpRequestParseState;
using tiny_muduo::Method;

HttpRequest::HttpRequest() {}

HttpRequest::~HttpRequest() {}

bool HttpRequest::ParseRequestMethod(char const *start, char const *end) {
    string method(start, end);
    bool has_method = true;
    if (method == "GET") {
        method_ = kGet;
    } else if (method == "POST") {
        method_ = kPost;
    } else if (method == "PUT") {
        method_ = kPut;
    } else if (method == "DELETE") {
        method_ = kDelete;
    } else if (method == "TRACE") {
        method_ = kTrace;
    } else if (method == "OPTIONS") {
        method_ = kOptions;
    } else if (method == "CONNECT") {
        method_ = kConnect;
    } else if (method == "PATCH") {
        method_ = kPatch;
    } else {
        has_method = false;
    }

    return has_method;
}

void HttpRequest::ParseRequestLine(char const *start, char const *end,
                                   HttpRequestParseState &state) {
    {
        char const *space = std::find(start, end, ' ');
        if (space == end) {
            state = kParseErrno;
            return;
        }

        if (!ParseRequestMethod(start, space)) {
            state = kParseErrno;
            return;
        }
        start = space + 1;
    }

    {
        char const *space = std::find(start, end, ' ');
        if (space == end) {
            state = kParseErrno;
            return;
        }

        char const *query = std::find(start, end, '?');
        if (query != end) {
            path_ = std::move(string(start, query));
            query_ = std::move(string(query + 1, space));
        } else {
            path_ = std::move(string(start, space));
        }
        start = space + 1;
    }

    {
        int const httplen = sizeof(http) / sizeof(char) - 1;
        char const *httpindex = std::search(start, end, http, http + httplen);
        if (httpindex == end) {
            state = kParseErrno;
            return;
        }

        char const chr = *(httpindex + httplen);
        if (httpindex + httplen + 1 == end && (chr == '1' || chr == '0')) {
            if (chr == '1') {
                version_ = kHttp11;
            } else {
                version_ = kHttp10;
            }
        } else {
            state = kParseErrno;
            return;
        }
    }

    state = kParseHeaders;
}

void HttpRequest::ParseBody(char const *start, char const *end,
                            HttpRequestParseState &state) {}

void HttpRequest::ParseHeaders(char const *start, char const *end,
                               HttpRequestParseState &state) {
    if (start == end && *start == '\r' && *(start + 1) == '\n') {
        state = kParseGotCompleteRequest;
        return;
    }

    char const *colon = std::find(start, end, ':');
    if (colon == end) {
        state = kParseErrno;
        return;
    }

    char const *vaild = colon + 1;
    while (*(vaild++) != ' ') {}
    headers_[std::move(string(start, colon))] =
        std::move(string(colon + 1, vaild));
    return;
}
