#pragma once
#include "Animation.h"

class ParallelAnimation :
    public Animation
{
public:
    static AnimationPtr create(const std::list<AnimationPtr>& anims)
    {
        AnimationPtr ret(new ParallelAnimation(anims));
        return ret;
    }

    virtual void render()
    {
        for(std::list<AnimationPtr>::iterator it = anims_.begin(); it != anims_.end(); it++)
        {
            (*it)->render();
            if ((*it)->isDone())
                it = anims_.erase(it);
        }
    }

    virtual void isDone()
    {
        return anims_.empty();
    }

private:
    ParallelAnimation(const std::list<AnimationPtr>& a) :
        anims_(anims)
    { /* Empty */ }

    std::list<AnimationPtr> anims_;

};

