#ifndef TINY_MUDUO_BUFFER_H_
#define TINY_MUDUO_BUFFER_H_

#include <algorithm>
#include <string>
#include <vector>

#define NDEBUG
#include <assert.h>
#include <cstring>

using std::string;

namespace tiny_muduo {

static int const kPrePendIndex = 8;

class Buffer {
public:
    Buffer()
        : buffer_(1024),
          read_index_(kPrePendIndex),
          write_index_(kPrePendIndex) {}

    ~Buffer() {}

    int ReadFd(int fd);

    char *begin() {
        return &*buffer_.begin();
    };

    char const *begin() const {
        return &*buffer_.begin();
    };

    char *beginread() {
        return begin() + read_index_;
    }

    char const *beginread() const {
        return begin() + read_index_;
    }

    char *beginwrite() {
        return begin() + write_index_;
    }

    char const *beginwrite() const {
        return begin() + write_index_;
    }

    void Append(char const *message) {
        Append(message, strlen(message));
    }

    void Append(char const *message, int len) {
        MakeSureEnoughStorage(len);
        std::copy(message, message + len, beginwrite());
        write_index_ += len;
    }

    void Append(string const &message) {
        Append(message.data(), message.size());
    }

    void Retrieve(int len) {
        assert(readablebytes() >= len);
        if (len + read_index_ < write_index_) {
            read_index_ = read_index_ + len;
        } else {
            write_index_ = kPrePendIndex;
            read_index_ = write_index_;
        }
    }

    void RetrieveUntilIndex(char const *index) {
        assert(beginwrite() >= index);
        read_index_ += index - beginread();
    }

    void RetrieveAll() {
        write_index_ = kPrePendIndex;
        read_index_ = write_index_;
    }

    string RetrieveAsString(int len) {
        assert(read_index_ + len <= write_index_);
        string ret = std::move(PeekAsString(len));
        Retrieve(len);
        return ret;
    }

    string RetrieveAllAsString() {
        string ret = std::move(PeekAllAsString());
        RetrieveAll();
        return ret;
    }

    char const *Peek() const {
        return beginread();
    }

    char *Peek() {
        return beginread();
    }

    string PeekAsString(int len) {
        return string(beginread(), beginread() + len);
    }

    string PeekAllAsString() {
        return string(beginread(), beginwrite());
    }

    int readablebytes() const {
        return write_index_ - read_index_;
    }

    int writablebytes() const {
        return buffer_.capacity() - write_index_;
    }

    int prependablebytes() const {
        return read_index_;
    }

    void MakeSureEnoughStorage(int len) {
        if (writablebytes() >= len) {
            return;
        }
        if (writablebytes() + prependablebytes() >= kPrePendIndex + len) {
            std::copy(beginread(), beginwrite(), begin() + kPrePendIndex);
            write_index_ = kPrePendIndex + readablebytes();
            read_index_ = kPrePendIndex;
        } else {
            buffer_.resize(buffer_.capacity() + len);
        }
    }

private:
    std::vector<char> buffer_;
    int read_index_;
    int write_index_;
};

} // namespace tiny_muduo
#endif
