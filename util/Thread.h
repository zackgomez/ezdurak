#pragma once
#include <pthread.h>

class Thread
{
public:
    Thread() : running_(false)
    { /* Empty */ }

    ~Thread()
    {
        kill();
    }

    void run( void* f(void*), void * arg)
    {
        pthread_create(&t_, NULL, f, arg);
        running_ = true;
    }

    void kill()
    {
        if (running_)
        {
            pthread_cancel(t_);
            pthread_join(t_, NULL);
        }
    }

private:
    Thread(const Thread&);
    const Thread& operator=(const Thread&);

    bool running_;
    pthread_t t_;
};

class CondVar;

class Mutex
{
public:
    Mutex()
    {
        pthread_mutex_init(&m_, NULL);
    }

    ~Mutex()
    {
        pthread_mutex_destroy(&m_);
    }

    void lock()
    {
        pthread_mutex_lock(&m_);
    }

    void unlock()
    {
        pthread_mutex_unlock(&m_);
    }

    friend class CondVar;

private:
    pthread_mutex_t m_;
};

class Lock
{
public:
    Lock(Mutex &m) : m_(m)
    {
        m_.lock();
    }

    ~Lock()
    {
        m_.unlock();
    }

private:
    Mutex &m_;
};

class CondVar
{
public:
    CondVar()
    {
       pthread_cond_init(&c_, NULL);
    }
    ~CondVar()
    {
        pthread_cond_destroy(&c_);
    }

    void wait(Mutex &m)
    {
        pthread_cond_wait(&c_, &m.m_);
    }
    void signal()
    {
        pthread_cond_signal(&c_);
    }
    
private:
    pthread_cond_t c_;
};
