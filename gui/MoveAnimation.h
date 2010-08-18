#pragma once
#include "Animation.h"

class CardHolder;

class MoveAnimation :
    public Animation
{
public:
    static AnimationPtr create(Card c, CardHolder *source, CardHolder *target,
                               int duration, int x0, int y0, int x1, int y1);
    virtual ~MoveAnimation();

    virtual bool isDone() const;
    virtual void render();

private:
    // Private constructor for create idiom
    MoveAnimation(Card c, CardHolder *source, CardHolder *target, int duration,
                  int x0, int y0, int x1, int y1);

    Card card_;
    CardHolder *source_;
    CardHolder *target_;
    int elapsed_;
    int duration_;

    int x0_, y0_;
    int x1_, y1_;

    bool removed_;
};