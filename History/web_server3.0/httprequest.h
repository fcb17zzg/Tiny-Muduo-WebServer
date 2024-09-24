#ifndef TINY_MUDUO_HTTPREQUEST_H_
#define TINY_MUDUO_HTTPREQUEST_H_

#include "httpparsestate.h"
#include <map>
#include <string>

using std::string;

namespace tiny_muduo {

static char const http[] = "HTTP/1.";

enum Method {
    kGet,
    kPost,
    kPut,
    kDelete,
    kTrace,
    kOptions,
    kConnect,
    kPatch
};

enum Version {
    kHttp10,
    kHttp11
};

class HttpRequest {
public:
    HttpRequest();
    ~HttpRequest();

    bool ParseRequestMethod(char const *start, char const *end);

    void ParseRequestLine(char const *start, char const *end,
                          HttpRequestParseState &state);

    void ParseHeaders(char const *start, char const *end,
                      HttpRequestParseState &state);

    void ParseBody(char const *start, char const *end,
                   HttpRequestParseState &state);

    Method method() const {
        return method_;
    }

    string const &path() const {
        return path_;
    }

    string const &query() const {
        return query_;
    }

    Version version() const {
        return version_;
    }

    std::map<string, string> const &headers() const {
        return headers_;
    }

    string GetHeader(string const &header) const {
        auto iter = headers_.find(header);
        if (iter == headers_.end()) {
            return string();
        } else {
            return iter->second;
        }
    }

private:
    Method method_;
    string path_;
    string query_;
    Version version_;
    std::map<string, string> headers_;
};

} // namespace tiny_muduo
#endif
