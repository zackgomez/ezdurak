#pragma once
#include "GameAgent.h"
#include <vector>
#include <set>
#include "Deck.h"

class Player;

class Game : public GameAgent
{
public:
    Game(int numPlayers);
    virtual ~Game();

    void run();

    // Functions inherited from GameAgent interface
    virtual void addListener(GameListener *listener);
    virtual void removeListener(GameListener *listener);
    virtual Card::cardsuit getTrump() const;
    virtual const Player * getAttacker() const;
    virtual const Player * getDefender() const;
    virtual const std::vector<Player*>& getPlayers() const;

    virtual const std::vector<Card>& getPlayedCards() const;

private:
    // Helper Functions
    /**
     * Returns true if it was a successful defend.
     */
    bool doRound();
    /**
     * Gives each player, in the order they played, with the defender last, 
     * cards until they have at least 6 or the deck runs out.
     */
    void refillCards();

    /**
     * Sets attacker_ to the next attacker unless that would be the defender
     * in which case it skips them and goes to the next person.
     * Depends on attacker_,defenderIdx_, and nextAttackerIdx_
     */
    void nextAttacker();

    /**
     * Gives the defender extra cards with a maximum of maxCards starting with
     * the current attacker going first.
     */
    void pileOn(int maxCards);

    /**
     * Returns false if the hand is not a valid starting hand for Durak.  I.e. 
     * the hand contains more than 4 of one suit or is all one color.
     * @return false if the hand is invalid
     */
    bool validateHand(const std::vector<Card>& hand);

    // GameAgent implementation members
    std::set<GameListener*> listeners_;

    // Data Members
    std::vector<Player*> players_;
    int attackerIdx_;
    int defenderIdx_;
    Deck deck_;
    Card::cardsuit trump_;
    Card trumpCard_;

    // Transient Members
    Player *attacker_;
    Player *defender_;

    int nextAttackerIdx_;

    std::vector<Card> playedCards_;
    std::set<int> playedRanks_;
};
