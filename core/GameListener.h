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

    /**
     * Called at the start of a new round.  I.e. when there is a new defender.
     * @param attacker The starting attacker for the round.
     * @param defender The starting defender for the round.
     */
    virtual void newRound(ConstPlayerPtr attacker, ConstPlayerPtr defender) = 0;
    /**
     * When an attacker passes initiative to another, this function is called.
     * @param newAttacker The new attacker with initiative.
     */
    virtual void attackerPassed(ConstPlayerPtr newAttacker) = 0;

    /**
     * Called when either the defender has given up, or successfully defended
     * all of the attacking cards.
     * @param successfulDefend True if the defender successfully defending the
     * attacking cards.
     */
    virtual void endRound(bool successfulDefend) = 0;

    /**
     * Called when an attacking card is played on the table.
     * @param c The attacking card.
     */
    virtual void attackingCard(const Card &c) = 0;
    /**
     * Called when a defending card is played on the table.
     * @param c The attacking card.
     */
    virtual void defendingCard(const Card &c) = 0;
    /**
     * Called when an attacker piles a card on to the defender.
     * @param c The piled on card.
     */
    virtual void piledOnCard(const Card &c) = 0;

    /**
     * Called after the players are given cards if a player has no cards 
     * remaining and has "gone out" of the game.
     * @param player The player that has gone out.
     */
    virtual void playedOut(ConstPlayerPtr player) = 0;

    /**
     * Called whenever a player is given cards.  There are two times this
     * occurs, when a defender loses and takes the pile, or when at the end of
     * a round when players fill up on cards.
     * @param player The player recieving the cards.
     * @param numCards The number of cards recieved.
     */
    virtual void givenCards(ConstPlayerPtr player, int numCards) = 0;
};
