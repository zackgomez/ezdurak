#pragma once
#include "core/GameListener.h"
#include <set>
#include <map>

/** 
 * This is a utility class that 'counts cards'.  It keeps track of cards that
 * have been discarded or given to players.  It provides facilities for querying
 * the gathered information as well as getting to the raw data.
 */
class CardCounter :
    public GameListener
{
public:
    CardCounter();
    ~CardCounter();

    typedef std::set<Card> cardset;

    /** 
     * Returns true if the passed card is in the discard pile.
     * 
     * @param c Card to check
     * 
     * @return True if the card is dead.
     */
    bool isDiscarded(const Card& c) const;

    /**
     * Returns the set of cards that are discarded.
     *
     * @return The set of cards discarded.
     */
    const cardset& discardedCards() const;

    /** 
     * Returns a list of the known cards a player has.  This information is
     * gathered when a player unsuccessfully defends and takes known cards.
     * It is updated when a player plays a card.
     * 
     * @param p The player to query against.
     * 
     * @return The known cards for that player
     */
    cardset knownCards(ConstPlayerPtr p) const;

    // Methods from GameListener interface
    void gameStart(GameAgent *agent);
    void gameOver(const ConstPlayerPtr biscuit);
    void newRound(const ConstPlayerPtr, const ConstPlayerPtr);
    void attackerPassed(const ConstPlayerPtr);
    void endRound(bool);
    void attackingCard(const Card &);
    void defendingCard(const Card &);
    void piledOnCard(const Card &);
    void playedOut(ConstPlayerPtr);
    void givenCards(ConstPlayerPtr, int);
    void givenCards(ConstPlayerPtr, const std::vector<Card>&);

private:
    cardset playedCards_;
    cardset discardedCards_;
    std::map<ConstPlayerPtr, cardset> playerCards_;

    ConstPlayerPtr attacker_, defender_;
    GameAgent* agent_;
};

