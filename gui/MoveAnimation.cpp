#include "MoveAnimation.h"
#include "CardHolder.h"
#include "gl.h"
#include "GUICard.h"

MoveAnimation::MoveAnimation(Card c, CardHolder *t, int d, int x0, int y0, int x1, int y1) :
    card_(c),
    target_(t),
    elapsed_(0),
    duration_(d),
    x0_(x0), y0_(y0),
    x1_(x1), y1_(y1)
{ /* Empty */ }

MoveAnimation::~MoveAnimation()
{
    target_->addCard(card_);
}

bool MoveAnimation::isDone() const
{
    return elapsed_ >= duration_;
}

void MoveAnimation::render()
{
    // Position
    float x = x0_ + (x1_ - x0_) * ((float) elapsed_ / duration_);
    float y = y0_ + (y1_ - y0_) * ((float) elapsed_ / duration_);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(x, y, 0);

    glColor3f(1,1,1);
    GUICard::draw(card_);
    glPopMatrix();

    elapsed_++;
}

AnimationPtr MoveAnimation::create(Card c, CardHolder *t, int d, int x0, int y0, int x1, int y1)
{
    AnimationPtr ret(new MoveAnimation(c, t, d, x0, y0, x1, y1));
    return ret;
}
