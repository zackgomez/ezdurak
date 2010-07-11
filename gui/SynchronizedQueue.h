#pragma once
#include <pthread.h>
#include <queue>

template<typename T>
class SynchronizedQueue
{
public:
    SynchronizedQueue() :
        queue_()
    {
        pthread_mutex_init(&lock_, NULL);
        pthread_cond_init(&cond_, NULL);
    }

    ~SynchronizedQueue()
    {
        pthread_mutex_destroy(&lock_);
        pthread_cond_destroy(&cond_);
    }

    T dequeue()
    {
        pthread_mutex_lock(&lock_);
        if (queue_.empty())
            pthread_cond_wait(&cond_, &lock_);

        T ret = queue_.front();
        queue_.pop();
        pthread_mutex_unlock(&lock_);
        return ret;
    }

    void enqueue(const T& val)
    {
        pthread_mutex_lock(&lock_);
        queue_.push(val);
        pthread_cond_signal(&cond_);
        pthread_mutex_unlock(&lock_);
    }

    void clear()
    {
        pthread_mutex_lock(&lock_);
        while (!queue_.empty())
            queue_.pop();
        pthread_mutex_unlock(&lock_);
    }

    bool empty() const
    {
        bool ret;
        pthread_mutex_lock(&lock_);
        ret = queue_.empty();
        pthread_mutex_unlock(&lock_);
        return ret;
    }

private:
    std::queue<T>   queue_;
    pthread_mutex_t lock_;
    pthread_cond_t  cond_;
};
