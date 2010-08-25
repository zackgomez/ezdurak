#include "Animation.h"
#include "PlayedCardsView.h"
#include "GUICard.h"


// -----------------------------------------------------------------------------
// Status Change Animations
// -----------------------------------------------------------------------------
AnimationPtr StatusChangeAnimation::create(GUIPlayerView *p1, GUIPlayerView::Status s1,
                                           GUIPlayerView *p2, GUIPlayerView::Status s2)
{
    AnimationPtr ret(new StatusChangeAnimation(p1, s1, p2, s2));
    return ret;
}
AnimationPtr StatusChangeAnimation::create(GUIPlayerView *p1, GUIPlayerView::Status s1)
{
    AnimationPtr ret(new StatusChangeAnimation(p1, s1,
                                               NULL, GUIPlayerView::NONE));
    return ret;
}

 void StatusChangeAnimation::render()
{
    if (p1_)
        p1_->setStatus(s1_);
    if (p2_)
        p2_->setStatus(s2_);

    done_ = true;
}

 bool StatusChangeAnimation::isDone() const
{
    return done_;
}

StatusChangeAnimation::StatusChangeAnimation(GUIPlayerView *p1, GUIPlayerView::Status s1,
                                             GUIPlayerView *p2, GUIPlayerView::Status s2) :
    p1_(p1), p2_(p2),
    s1_(s1), s2_(s2),
    done_(false)
{
    done_ = !p1_ && !p2_;
}

// -----------------------------------------------------------------------------
// Clear Animations - Clears a CardHolder
// -----------------------------------------------------------------------------
AnimationPtr ClearAnimation::create(CardHolder *target)
{
    AnimationPtr ret(new ClearAnimation(target));
    return ret;
}

bool ClearAnimation::isDone() const
{
    return done_;
}

 void ClearAnimation::render()
{
    target_->clear();
    done_ = true;
}

ClearAnimation::ClearAnimation(CardHolder *target) : done_(false), target_(target)
{ /* Empty */ }

// -----------------------------------------------------------------------------
// Delay Animation - Does nothing for a number of frames
// -----------------------------------------------------------------------------
AnimationPtr DelayAnimation::create(int duration)
{
    AnimationPtr ret(new DelayAnimation(duration));
    return ret;
}

void DelayAnimation::render()
{
    --remaining_;
}

bool DelayAnimation::isDone() const
{
    return remaining_ <= 0;
}

DelayAnimation::DelayAnimation(int duration) :
    remaining_(duration)
{ /* Empty */ }

// -----------------------------------------------------------------------------
// Parallel Animation - Runs one or more animations in parallel
// -----------------------------------------------------------------------------
AnimationPtr ParallelAnimation::create(const std::list<AnimationPtr>& anims)
{
    AnimationPtr ret(new ParallelAnimation(anims));
    return ret;
}

 void ParallelAnimation::render()
{
    for(std::list<AnimationPtr>::iterator it = anims_.begin(); it != anims_.end(); it++)
    {
        (*it)->render();
        if ((*it)->isDone())
        {
            it = anims_.erase(it);
            if (it == anims_.end())
                break;
        }
    }
}

 bool ParallelAnimation::isDone() const
{
    return anims_.empty();
}

ParallelAnimation::ParallelAnimation(const std::list<AnimationPtr>& a) :
    anims_(a)
{ /* Empty */ }

// -----------------------------------------------------------------------------
// Set Animation - Sets a boolean variable to true
// -----------------------------------------------------------------------------
AnimationPtr SetAnimation::create(bool &var)
{
    AnimationPtr ret(new SetAnimation(var));
    return ret;
}

 void SetAnimation::render()
{
    var_ = true;
}

 bool SetAnimation::isDone() const
{
    return var_;
}

SetAnimation::SetAnimation(bool &var) : var_(var)
{ /* Empty */ }


// -----------------------------------------------------------------------------
// Move Animation - Moves a card from one location/CardHolder to another
// -----------------------------------------------------------------------------
AnimationPtr MoveAnimation::create(Card c, CardHolder *source, CardHolder *target,
                                   int duration, int x0, int y0, int x1, int y1)
{
    AnimationPtr ret(new MoveAnimation(c, source, target, duration,
                                       x0, y0, x1, y1));
    return ret;
}

 bool MoveAnimation::isDone() const
{
    return elapsed_ >= duration_;
}

 void MoveAnimation::render()
{
    // Remove from the source on the first frame
    if (elapsed_ == 0 && source_)
        source_->removeCard(card_);

    // Position
    float x = x0_ + (x1_ - x0_) * ((float) elapsed_ / duration_);
    float y = y0_ + (y1_ - y0_) * ((float) elapsed_ / duration_);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(x, y, 0);

    glColor3f(1,1,1);
    if (card_)
        GUICard::draw(card_);
    else
        GUICard::drawCardBack();
    glPopMatrix();

    elapsed_++;
    // Add the card on the last frame
    if (elapsed_ >= duration_)
        target_->addCard(card_);
}

MoveAnimation::MoveAnimation(Card c, CardHolder *s, CardHolder *t, int d,
                             int x0, int y0, int x1, int y1) :
    card_(c),
    source_(s),
    target_(t),
    elapsed_(0),
    duration_(d),
    x0_(x0), y0_(y0),
    x1_(x1), y1_(y1)
{ /* Empty */ }

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
    {
        if (cards_.size() == 0)
            anim_ = pc_->getAnimation(target_, dur_, x1_, y1_);
        else
            anim_ = pc_->getAnimation(cards_, target_, dur_, x1_, y1_);
    }
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
