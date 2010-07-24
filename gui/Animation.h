#pragma once
#include "GUICard.h"

class Animation;
typedef boost::shared_ptr<Animation> AnimationPtr;

class Animation
{
public:
    virtual ~Animation();

    virtual void draw() = 0;
    virtual bool isDone() = 0;
};

class DelayAnimation : public Animation
{
public:
    static AnimationPtr create(int frames);

    virtual void draw();
    virtual bool isDone();

private:
    DelayAnimation(int frames);
    int framesLeft_;
};

class MoveAnimation : public Animation
{
public:
    static AnimationPtr create(GUICardPtr card, int xo, int yo, int frames);

    virtual void draw();
    virtual bool isDone();

private:
    MoveAnimation(GUICardPtr card, int xorigin, int yorigin, int frames);
    GUICardPtr card_;
    int xorigin_, yorigin_;
    int framesLeft_;
};
