#pragma once
#include "GameAgent.h"

class Player;

class GameListener
{
public:
    GameListener(GameAgent *agent) : agent_(agent) { agent_->addListener(this); }
    /** All intended base classes must have virtual destructors. */
    virtual ~GameListener() { agent_->removeListener(this); }

    virtual void attackerChanged(const Player* newAttacker) = 0;
    virtual void defenderChanged(const Player* newDefender) = 0;
    virtual void defenderLost() = 0;
    virtual void defenderWon() = 0;

    virtual void attackingCard(const Card &c) = 0;
    virtual void defendingCard(const Card &c) = 0;

    virtual void playedOut(const Player *player) = 0;
    virtual void givenCards(const Player *player, int numCard) = 0;

protected:
    GameAgent *agent_;
};
