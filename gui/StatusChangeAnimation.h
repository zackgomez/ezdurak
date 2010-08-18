#pragma once
#include "Animation.h"

class StatusChangeAnimation :
    public Animation
{
public:
    static AnimationPtr create(GUIPlayerView *p1, GUIPlayerView::Status s1,
                                GUIPlayerView *p2, GUIPlayerView::Status s2)
    {
        AnimationPtr ret(new StatusChangeAnimation(p1, s1, p2, s2));
        return ret;
    }
    static AnimationPtr create(GUIPlayerView *p1, GUIPlayerView::Status s1)
    {
        AnimationPtr ret(new StatusChangeAnimation(p1, s1,
                                                   NULL, GUIPlayerView::NONE));
        return ret;
    }

    virtual void render()
    {
        if (p1_)
            p1_->setStatus(s1_);
        if (p2_)
            p2_->setStatus(s2_);

        done_ = true;
    }

    virtual bool isDone() const
    {
        return done_;
    }

private:
    StatusChangeAnimation(GUIPlayerView *p1, GUIPlayerView::Status s1,
                          GUIPlayerView *p2, GUIPlayerView::Status s2) :
        p1_(p1), p2_(p2),
        s1_(s1), s2_(s2),
        done_(false)
    {
        done_ = !p1_ && !p2_;
    }

    GUIPlayerView *p1_, *p2_;
    GUIPlayerView::Status s1_, s2_;
    bool done_;
};

