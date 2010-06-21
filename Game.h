#pragma once
#include <vector>
#include <set>
#include "Deck.h"

class GamePlayer;

class Game
{
public:
    Game(int numPlayers);
    ~Game();

    void run();

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

    // Data Members
    std::vector<GamePlayer*> players_;
    int attackerIdx_;
    int defenderIdx_;
    Deck deck_;
    Card::cardsuit trump_;
    Card trumpCard_;

    // Transient Members
    GamePlayer *attacker_;
    GamePlayer *defender_;

    int nextAttackerIdx_;

    std::vector<Card> playedCards_;
};
