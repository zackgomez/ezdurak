#pragma once
#include "GameAgent.h"

class Player;

class GameListener
{
public:
    GameListener(GameAgent *agent) : agent_(agent) { agent_->addListener(this); }
    /** All intended base classes must have virtual destructors. */
    virtual ~GameListener() { agent_->removeListener(this); }

    /**
     * This function is called after the Game is completely initialized and 
     * the first attacker is just about to start.  It would be a good time to
     * get starting information from the game and do some initialization.
     */
    virtual void gameStart() = 0;

    /**
     * Called when the game is over.
     * @param biscuit The losing "biscuit" player or NULL if it was a tie.
     */
    virtual void gameOver(const Player* biscuit) = 0;

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
