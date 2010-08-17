#pragma once
#include "core/Card.h"
#include <boost/shared_ptr.hpp>

class CardHolder;
class Animation;

typedef boost::shared_ptr<Animation> AnimationPtr;

class Animation
{
public:
    // Virtual Destructor for intended base classes
    virtual ~Animation() { /* Empty */ }

    virtual bool isDone() const = 0;
    virtual void render() = 0;
};

