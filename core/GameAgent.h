#pragma once
#include <vector>
#include "Card.h"
#include "Player.h"

class GameListener;

/** Queryable interface that GameListeners are exposed to. */
class GameAgent
{
public:
    /** All intended base classes must have virtual destructors. */
    virtual ~GameAgent() {}

    /**
     * Add a listener to this game agent to recieve messages.
     */
    virtual void addListener(GameListener* listener) = 0;
    /**
     * Remove a listener if they are currently listening, otherwise do nothing.
     */
    virtual void removeListener(GameListener* listener) = 0;

    /**
     * Returns the card the defines trump.
     */
    virtual Card getTrumpCard() const = 0;

    /**
     * Returns the number of cards left in the deck.
     */
    virtual int getDeckSize() const = 0;

    /**
     * Returns the number of cards that are not in the deck or in a player's 
     * hand.  I.e. "Dead cards".
     */
    virtual int getDiscardSize() const = 0;

    /**
     * Returns the number of tricks left to play on the current defender.
     */
    virtual int getTricksLeft() const = 0;

    /** 
     * Returns the current attacker.
     */
    virtual ConstPlayerPtr getAttacker() const = 0;

    /** 
     * Returns the current attacker.
     */
    virtual ConstPlayerPtr getDefender() const = 0;

    /**
     * Returns the list of active players in seated order.  You can query
     * the players for the number of cards they have remaining in their hand
     * or their name.
     */
    virtual const std::vector<PlayerPtr> getPlayers() const = 0;

    /**
     * Returns the played cards for the attack against the current defender.
     * This is reset each round sometime after the defenderWon/Lost message
     * is sent.
     */
    virtual const std::vector<Card>& getPlayedCards() const = 0;
};

