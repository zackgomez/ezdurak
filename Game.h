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
    const Player * getAttacker() const;
    const Player * getDefender() const;
    const std::vector<Player*>& getPlayers() const;
    const std::vector<Card>& getPlayedCards() const;

private:
    Deck deck_;
    Card trumpCard_;
    std::vector<Player*> players_;

    Player *attacker_;
    Player *defender_;
    // TODO:2010-06-30:zack: Rename this variable to be clearer.
    // It should signify that it is the number of attacking cards that can be
    // played.
    int tricksLeft_;
    std::vector<Card> playedCards_;
    std::set<int> playableRanks_;

    std::set<GameListener*> listeners_;

    /** Deals a the initial hands to the players */
    void deal();
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
    /** Used by nextAttacker. */
    int attackerIdx_;
};

