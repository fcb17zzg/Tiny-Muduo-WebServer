#ifndef TINY_MUDUO_ADDRESS_H_
#define TINY_MUDUO_ADDRESS_H_

#include <cstring>
#include <string>

namespace tiny_muduo {

class Address {
private:
    char const *ip_;
    int const port_;

public:
    Address(char const *port = "2022") : ip_("127.0.0.1"), port_(atoi(port)) {}

    char const *ip() const {
        return ip_;
    }

    int const port() const {
        return port_;
    }
};

} // namespace tiny_muduo
#endif
