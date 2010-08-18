#pragma once
#include "Animation.h"

class DelayAnimation :
    public Animation
{
public:
    static AnimationPtr create(int duration)
    {
        AnimationPtr ret(new DelayAnimation(duration));
        return ret;
    }

    void render()
    {
        --remaining_;
    }

    bool isDone() const
    {
        return remaining_ <= 0;
    }

private:
    DelayAnimation(int duration) :
        remaining_(duration)
    { /* Empty */ }

    int remaining_;
};
