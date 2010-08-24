#pragma once
#include "core/Card.h"
#include <boost/shared_ptr.hpp>
#include <list>
#include "util/Thread.h"
#include "GUICard.h"
#include "GUIPlayerView.h"

class Animation;
class CardHolder;
typedef boost::shared_ptr<Animation> AnimationPtr;

// -----------------------------------------------------------------------------
// Base Animation Class
// -----------------------------------------------------------------------------
class Animation
{
public:
    // Virtual Destructor for intended base classes
    virtual ~Animation() { /* Empty */ }

    virtual bool isDone() const = 0;
    virtual void render() = 0;
};


// -----------------------------------------------------------------------------
// Status Change Animations
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Clear Animations - Clears a CardHolder
// -----------------------------------------------------------------------------
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


// -----------------------------------------------------------------------------
// Delay Animation - Does nothing for a number of frames
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Parallel Animation - Runs one or more animations in parallel
// -----------------------------------------------------------------------------
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
            {
                it = anims_.erase(it);
                if (it == anims_.end())
                    break;
            }
        }
    }

    virtual bool isDone() const
    {
        return anims_.empty();
    }

private:
    ParallelAnimation(const std::list<AnimationPtr>& a) :
        anims_(a)
    { /* Empty */ }

    std::list<AnimationPtr> anims_;

};


// -----------------------------------------------------------------------------
// Set Animation - Sets a boolean variable to true
// -----------------------------------------------------------------------------
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


// -----------------------------------------------------------------------------
// Move Animation - Moves a card from one location/CardHolder to another
// -----------------------------------------------------------------------------
class MoveAnimation :
    public Animation
{
public:
    static AnimationPtr create(Card c, CardHolder *source, CardHolder *target,
                               int duration, int x0, int y0, int x1, int y1)
    {
        AnimationPtr ret(new MoveAnimation(c, source, target, duration,
                                           x0, y0, x1, y1));
        return ret;
    }


    virtual ~MoveAnimation()
    {
    }

    virtual bool isDone() const
    {
        return elapsed_ >= duration_;
    }

    virtual void render()
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

private:
    // Private constructor for create idiom
    MoveAnimation(Card c, CardHolder *s, CardHolder *t, int d,
                  int x0, int y0, int x1, int y1) :
        card_(c),
        source_(s),
        target_(t),
        elapsed_(0),
        duration_(d),
        x0_(x0), y0_(y0),
        x1_(x1), y1_(y1)
    { /* Empty */ }

    // Data Members 
    Card card_;
    CardHolder *source_;
    CardHolder *target_;
    int elapsed_;
    int duration_;

    int x0_, y0_;
    int x1_, y1_;
};

// -----------------------------------------------------------------------------
// Synchronization Animation - Signals a condition variable
// -----------------------------------------------------------------------------
class SynchronizationAnimation :
    public Animation
{
public:
    static AnimationPtr create(CondVar *cond)
    {
        AnimationPtr ret(new SynchronizationAnimation(cond));
        return ret;
    }

    bool isDone() const
    {
        return signalled_;
    }
    void render()
    {
        cond_->signal();
        signalled_ = true;
    }

private:
    // Private constructor for create idiom
    SynchronizationAnimation(CondVar *c) :
        cond_(c),
        signalled_(false)
    { /* Empty */ }

    CondVar *cond_;
    bool signalled_;
};

