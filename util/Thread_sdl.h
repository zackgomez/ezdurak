#pragma once
#include <SDL/SDL_thread.h>

class Thread
{
public:
    Thread() : running_(false)
    { /* Empty */ }

    ~Thread()
    {
        kill();
    }

    void run( int f(void*), void * arg)
    {
        SDL_CreateThread(f, arg);
        running_ = true;
    }

    void kill()
    {
        if (running_)
        {
            SDL_KillThread(t_);
            SDL_WaitThread(t_, NULL);
        }
    }

private:
    Thread(const Thread&);
    const Thread& operator=(const Thread&);

    bool running_;
    SDL_Thread *t_;
};

class CondVar;

class Mutex
{
public:
    Mutex()
    {
        m_ = SDL_CreateMutex();
    }

    ~Mutex()
    {
        SDL_DestroyMutex(m_);
    }

    void lock()
    {
        SDL_mutexP(m_);
    }

    void unlock()
    {
        SDL_mutexV(m_);
    }

    friend class CondVar;

private:
    SDL_mutex *m_;
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
        c_ = SDL_CreateCond();
    }
    ~CondVar()
    {
        SDL_DestroyCond(c_);
    }

    void wait(Mutex &m)
    {
        SDL_CondWait(c_, m.m_);
    }
    void signal()
    {
        SDL_CondSignal(c_);
    }
    
private:
    SDL_cond *c_;
};
