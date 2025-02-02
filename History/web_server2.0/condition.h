#ifndef TINY_MUDUO_CONDITION_H_
#define TINY_MUDUO_CONDITION_H_

#include "mutex.h"
#include "pthread.h"

namespace tiny_muduo {
class Condition {
public:
    Condition(MutexLock &mutex) : mutex_(mutex) {
        pthread_cond_init(&cond_, nullptr);
    }

    ~Condition() {
        pthread_cond_destroy(&cond_);
    }

    bool Wait() {
        int ret = 0;
        ret = pthread_cond_wait(&cond_, mutex_.mutex());
        return ret == 0;
    }

    bool Signal() {
        return pthread_cond_signal(&cond_);
    }

    bool Broadcast() {
        return pthread_cond_broadcast(&cond_);
    }

private:
    MutexLock &mutex_;
    pthread_cond_t cond_;
};
} // namespace tiny_muduo
#endif // TINY_MUDUO_CONDITION_H_
