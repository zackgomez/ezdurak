#pragma once
#include "core/Card.h"
#include <boost/shared_ptr.hpp>

class CardHolder;
class Animation;

typedef boost::shared_ptr<Animation> AnimationPtr;

class Animation
{
public:
    static AnimationPtr create(Card c, CardHolder *target, int duration,
                        int x0, int y0, int x1, int y1);
    ~Animation();

    bool isDone() const;
    void render();

private:
    // Private constructor for create idiom
    Animation(Card c, CardHolder *target, int duration, int x0, int y0,
              int x1, int y1);

    Card card_;
    CardHolder *target_;
    int elapsed_;
    int duration_;

    int x0_, y0_;
    int x1_, y1_;
};
