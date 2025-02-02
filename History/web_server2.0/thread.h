#ifndef TINY_MUDUO_THREAD_H
#define TINY_MUDUO_THREAD_H

#include "latch.h"
#include <functional>
#include <pthread.h>

namespace tiny_muduo {
class Thread {
public:
    typedef std::function<void()> ThreadFunc;

    Thread(ThreadFunc const &func);
    ~Thread();

    void StartThread();

private:
    pthread_t pthread_id_;
    ThreadFunc func_;
    Latch latch_;
};

struct ThreadData {
    typedef tiny_muduo::Thread::ThreadFunc ThreadFunc;

    ThreadFunc func_;
    Latch *latch_;

    ThreadData(ThreadFunc &func, Latch *latch) : func_(func), latch_(latch) {}

    void RunOneFunc() {
        latch_->CountDown();
        latch_ = nullptr;
        func_();
    }
};
} // namespace tiny_muduo
#endif // TINY_MUDUO_THREAD_H
