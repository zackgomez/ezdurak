#pragma once
#include "Player.h"

class GameAgent;

class GameListener
{
public:
    GameListener() { /* Empty */ }
    /** All intended base classes must have virtual destructors. */
    virtual ~GameListener() { /* Empty */ }

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
    virtual void gameOver(ConstPlayerPtr biscuit) = 0;

    virtual void newRound(ConstPlayerPtr attacker, ConstPlayerPtr defender) = 0;
    virtual void attackerPassed(ConstPlayerPtr newAttacker) = 0;
    virtual void endRound(bool successfulDefend) = 0;

    virtual void attackingCard(const Card &c) = 0;
    virtual void defendingCard(const Card &c) = 0;
    virtual void piledOnCard(const Card &c) = 0;

    virtual void playedOut(ConstPlayerPtr player) = 0;
    virtual void givenCards(ConstPlayerPtr player, int numCard) = 0;
};
