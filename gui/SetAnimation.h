#pragma once
#include "Animation.h"

class SetAnimation :
    public Animation
{
public:
    static AnimationPtr create(bool &var)
    {
        AnimationPtr ret(new SetAnimation(var));
        return ret;
    }

    virtual void render()
    {
        var_ = true;
    }

    virtual bool isDone() const
    {
        return var_;
    }

private:
    SetAnimation(bool &var) : var_(var)
    { /* Empty */ }

    bool &var_;
};

