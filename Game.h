#pragma once
#include "GameAgent.h"
#include <vector>
#include <set>
#include "Deck.h"

class Player;

class Game : public GameAgent
{
public:
    Game(const std::vector<Player*>& players);
    ~Game();

    void run();

    // Constants
    const static int HAND_SIZE;

    // Functions inherited from GameAgent interface
    void addListener(GameListener *listener);
    void removeListener(GameListener *listener);
    Card getTrumpCard() const;
    int getTricksLeft() const;
    int getDeckSize() const;
    int getDiscardSize() const;
    const Player * getAttacker() const; const Player * getDefender() const; const std::vector<Player*>& getPlayers() const;
    const std::vector<Card>& getPlayedCards() const;

private:
    Deck deck_;
    Card trumpCard_;
    std::vector<Player*> players_;

    Player *attacker_;
    Player *defender_;
    int attackerIdx_;
    int defenderIdx_;
    int tricksLeft_;
    std::vector<Card> playedCards_;
    std::set<int> playableRanks_;
    std::vector<Player*> refillOrder_;

    std::set<GameListener*>::iterator lit_;
    std::set<GameListener*> listeners_;

    /** Deals a the initial hands to the players */
    void deal();
    /**
     * Returns true if a hand is a valid starting hand.  I.e. it is not a 
     * misdeal.
     */
    bool validateHands(const std::vector<std::vector<Card> >& hands) const;
    /**
     * Conducts one "round" vs a single defender.
     * @return True if the rounded ended due to a successful defend, false if 
     * the defender lost.
     */
    bool doRound();
    /** Gets an attacking card or the null card if the attackers all pass. */
    Card getAttackingCard();
    /** Moves to the next attacker, making sure to skip the current defender. */
    void nextAttacker();
    /**
     * Continues to put cards on playableCards until the attackers all pass or
     * the maximum number of tricks is reached.  It then gives all of the cards
     * played during the round and pile on to the defender.
     */
    void pileOn();
    /** Refills the players hands up to HAND_SIZE. */
    void refill();
    /**
     * Removes the players that have no cards left in their hand as they have 
     * gone out and are no longer playing.
     */
    void removeFinishedPlayers();
    /** Changes the defender_ variable to point to the next defender. */
    void nextDefender(bool successfulDefend);
};

