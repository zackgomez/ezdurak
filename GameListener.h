#pragma once
#include "GameAgent.h";

class GameListener
{
public:
    GameListener(GameAgent *agent);
    /** All intended base classes must have virtual destructors. */
    virtual ~GameListener() { agent_->removeListener(this); }

    virtual void attackerChanged() = 0;
    virtual void defenderLost() = 0;
    virtual void defenderWon() = 0;

    virtual void attackingCard(const Card &c) = 0;
    virtual void defendingCard(const Card &c) = 0;

protected:
    GameAgent *agent_;
};
