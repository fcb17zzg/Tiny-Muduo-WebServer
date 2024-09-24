#include "buffer.h"
#include <sys/uio.h>

using namespace tiny_muduo;

int Buffer::ReadFd(int fd) {
    char extrabuf[65536] = {0};
    struct iovec iv[2];
    int const writable = writablebytes();
    iv[0].iov_base = beginwrite();
    iv[0].iov_len = writable;
    iv[1].iov_base = extrabuf;
    iv[1].iov_len = sizeof(extrabuf);

    int const iovcnt = (writable < static_cast<int>(sizeof(extrabuf)) ? 2 : 1);
    int readn = ::readv(fd, iv, iovcnt);

    if (readn < 0) {
        printf("Buffer::ReadFd readn < 0 SYS_ERR\n");
    } else if (readn <= writable) {
        write_index_ += readn;
    } else {
        write_index_ = buffer_.size();
        Append(extrabuf, readn - writable);
    }

    return readn;
}
