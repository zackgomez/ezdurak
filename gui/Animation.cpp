#include "Animation.h"
#include "PlayedCardsView.h"

// -----------------------------------------------------------------------------
// Crunch Animation
// -----------------------------------------------------------------------------
AnimationPtr CrunchAnimation::create(PlayedCardsView *pc, CardHolder *target,
                    int duration, float x1, float y1)
{
    AnimationPtr ret(new CrunchAnimation(pc, target, duration, x1, y1));
    return ret;
}

AnimationPtr CrunchAnimation::create(PlayedCardsView *pc, const std::vector<Card> &cs,
                                     CardHolder *target, int duration, float x1, float y1)
{
    AnimationPtr ret(new CrunchAnimation(pc, cs, target, duration, x1, y1));
    return ret;
}

bool CrunchAnimation::isDone() const
{
    return generated_ && anim_->isDone();
}

void CrunchAnimation::render()
{
    if (!generated_)
        if (cards_.size() == 0)
            anim_ = pc_->getAnimation(target_, dur_, x1_, y1_);
        else
            anim_ = pc_->getAnimation(cards_, target_, dur_, x1_, y1_);
    generated_ = true;
    anim_->render();
}

CrunchAnimation::CrunchAnimation(PlayedCardsView *pc, CardHolder *t, int d, float x, float y) :
pc_(pc),
target_(t),
dur_(d),
x1_(x),
y1_(y),
generated_(false)
{
}

CrunchAnimation::CrunchAnimation(PlayedCardsView *pc, const std::vector<Card> &cards, 
                                 CardHolder *t, int d, float x, float y) :
pc_(pc),
cards_(cards),
target_(t),
dur_(d),
x1_(x),
y1_(y),
generated_(false)
{
}