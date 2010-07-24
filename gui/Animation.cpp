#include "Animation.h"

// --- Animation Base class methods ---
Animation::~Animation()
{ /* Empty */ }


// --- Delay Anmation methods ---
DelayAnimation::DelayAnimation(int frames) :
    framesLeft_(frames)
{ /* Empty */ }

AnimationPtr DelayAnimation::create(int frames)
{
    AnimationPtr ret(new DelayAnimation(frames));
    return ret;
}

void DelayAnimation::draw()
{
    --framesLeft_;
}

bool DelayAnimation::isDone()
{
    return framesLeft_ <= 0;
}

// --- Move Animation methods ---
MoveAnimation::MoveAnimation(GUICardPtr card, int xo, int yo, int frames) :
    card_(card),
    xorigin_(xo),
    yorigin_(yo),
    framesLeft_(frames)
{ /* Empty */ }

AnimationPtr MoveAnimation::create(GUICardPtr c, int xo, int yo, int frames)
{
    AnimationPtr ret(new MoveAnimation(c, xo, yo, frames));
    return ret;
}

void MoveAnimation::draw()
{
    // TODO
    --framesLeft_;
}

bool MoveAnimation::isDone()
{
    return framesLeft_ <= 0;
}
