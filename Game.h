#pragma once
#include "GameAgent.h"
#include <vector>
#include <set>
#include "Deck.h"

class Player;

class Game : public GameAgent
{
public:
    Game(const std::vector<Player*> players);
    virtual ~Game();

    void run();

    // Functions inherited from GameAgent interface
    void addListener(GameListener *listener);
    void removeListener(GameListener *listener);
    Card getTrumpCard() const;
    int  getTricksLeft() const;
    int  getDeckSize() const;
    
    const Player * getAttacker() const;
    const Player * getDefender() const;
    const std::vector<Player*>& getPlayers() const;

    const std::vector<Card>& getPlayedCards() const;

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
     * Gives the defender extra cards with a maximum of tricksLeft_.
     */
    void pileOn();

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
    int tricksLeft_;
};
