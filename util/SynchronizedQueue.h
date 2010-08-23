#pragma once
#include <queue>
#include "Thread.h"

template<typename T>
class SynchronizedQueue
{
public:
    SynchronizedQueue() :
        queue_(),
        done_(false)
    {
    }

    ~SynchronizedQueue()
    {
    }

    T dequeue()
    {
        Lock l(lock_);
        while (queue_.empty())
        {
            cond_.wait(lock_);
            if (done_)
                pthread_exit(NULL);
        }
        if (done_)
            pthread_exit(NULL);

        T ret = queue_.front();
        queue_.pop();
        return ret;
    }

    void enqueue(const T& val)
    {
        Lock l(lock_);
        queue_.push(val);
        cond_.signal();
    }

    void clear()
    {
        Lock l(lock_);
        while (!queue_.empty())
            queue_.pop();
    }

    bool empty() const
    {
        Lock l(lock_);
        bool ret;
        ret = queue_.empty();
        return ret;
    }

    void interrupt()
    {
        Lock l(lock_);
        cond_.signal();
    }

    void killReader()
    {
        Lock l(lock_);
        done_ = true;
        cond_.signal();
    }

private:
    std::queue<T> queue_;
    Mutex         lock_;
    CondVar       cond_;
    bool          done_;
};
