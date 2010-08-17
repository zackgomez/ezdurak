#pragma once
#include "Animation.h"

class ClearAnimation :
    public Animation
{
public:
    static AnimationPtr create(CardHolder *target)
    {
        AnimationPtr ret(new ClearAnimation(target));
        return ret;
    }

    virtual bool isDone() const
    {
        return done_;
    }

    virtual void render()
    {
        target_->clear();
        done_ = true;
    }

private:
    ClearAnimation(CardHolder *target) : done_(false), target_(target)
    { /* Empty */ }

    bool done_;
    CardHolder *target_;
};

