#pragma once
#include "core/Card.h"
#include <boost/shared_ptr.hpp>
#include <list>
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
                                GUIPlayerView *p2, GUIPlayerView::Status s2);
    static AnimationPtr create(GUIPlayerView *p1, GUIPlayerView::Status s1);

    virtual void render();

    virtual bool isDone() const;

private:
    StatusChangeAnimation(GUIPlayerView *p1, GUIPlayerView::Status s1,
                          GUIPlayerView *p2, GUIPlayerView::Status s2);

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
    static AnimationPtr create(CardHolder *target);

    virtual bool isDone() const;

    virtual void render();

private:
    ClearAnimation(CardHolder *target);

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
    static AnimationPtr create(int duration);

    void render();

    bool isDone() const;

private:
    DelayAnimation(int duration);

    int remaining_;
};

// -----------------------------------------------------------------------------
// Parallel Animation - Runs one or more animations in parallel
// -----------------------------------------------------------------------------
class ParallelAnimation :
    public Animation
{
public:
    static AnimationPtr create(const std::list<AnimationPtr>& anims);

    virtual void render();

    virtual bool isDone() const;

private:
    ParallelAnimation(const std::list<AnimationPtr>& a);

    std::list<AnimationPtr> anims_;

};


// -----------------------------------------------------------------------------
// Set Animation - Sets a boolean variable to true
// -----------------------------------------------------------------------------
class SetAnimation :
    public Animation
{
public:
    static AnimationPtr create(bool &var);

    virtual void render();

    virtual bool isDone() const;

private:
    SetAnimation(bool &var);

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
                               int duration, int x0, int y0, int x1, int y1);

    virtual bool isDone() const;

    virtual void render();

private:
    // Private constructor for create idiom
    MoveAnimation(Card c, CardHolder *s, CardHolder *t, int d,
                  int x0, int y0, int x1, int y1);

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
// Crunch Animation - "Crunches" the played cards to a given CardHolder and pos
// -----------------------------------------------------------------------------
// Forward Declaration
class PlayedCardsView;
class CrunchAnimation :
    public Animation
{
public:
    static AnimationPtr create(PlayedCardsView *pc, CardHolder *target,
        int duration, float x1, float y1);
    static AnimationPtr create(PlayedCardsView *pc, const std::vector<Card> &cards,
        CardHolder *target, int duration, float x1, float y1);

    bool isDone() const;

    void render();

private:
    CrunchAnimation(PlayedCardsView *pc, CardHolder *t, int d, float x, float y);
    CrunchAnimation(PlayedCardsView *pc, const std::vector<Card> &cs,
        CardHolder *t, int d, float x, float y);
          
    PlayedCardsView *pc_;
    const std::vector<Card> cards_;
    CardHolder *target_;
    int dur_;
    float x1_, y1_;

    bool generated_;
    AnimationPtr anim_;
};
