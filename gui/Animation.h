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
/** 
 * Base Animation class.  All Animations have two functions: render and isDone.
 * Render is a misnomer, just as Animation is.  render can be thought of as 
 * "act" or "doAction".  isDone is self explanatory.  Currently, you may be 
 * removed before render is ever called if isDone returns true.
 */
class Animation
{
public:
    // Virtual Destructor for intended base classes
    virtual ~Animation() { /* Empty */ }

    /** 
     * If this function returns true, the animation will be popped off of the
     * animation stack and deleted.
     * 
     * @return true if this animation is completed.
     */
    virtual bool isDone() const = 0;
    /** 
     * Called once per frame on the top most animation of the animation stack.
     */
    virtual void render() = 0;
};


// -----------------------------------------------------------------------------
// Status Change Animations
// -----------------------------------------------------------------------------
/** 
 * This animation changes the status on one or two players.  The two player
 * functionality is strictly a convienence and can be accomplished in other 
 * ways.
 */
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
/** 
 * This animation clears a CardHolder, removing all of the cards from it.
 */
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
/** 
 * This Animation does nothing for a specified number of frames, delaying the
 * animations after it on the Animation stack.
 */
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
/** 
 * Runs one or more animations in parallel.  The passed animations have their 
 * render method called once per frame, until they are 'isDone'.  When they are 
 * complete the are removed.  When there are no animations left, the 
 * ParallelAnimation is completed.
 */
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
/** 
 * This animation sets a boolean to true.  If the variable is already true, it
 * may not be written over again.
 */
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
/** 
 * This animations smoothly moves an animation from one CardHolder and location
 * to another.  Over the space of duration frames the Card is interpolated 
 * between the two endpoints.  On the first frame it is removed from the source
 * CardHolder and after the last frame it is added to the target CardHolder.
 */
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
/** 
 * This animation takes all of the cards from the PlayedCardsView and moves them
 * to another CardHolder.  The Cards are interpolated from their positions in 
 * the PlayedCardsView to the given position as in the MoveAnimation.
 */
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
