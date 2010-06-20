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
    void refillCards();

    // Data Members
    std::vector<GamePlayer*> players_;
    int attackerIdx_;
    int defenderIdx_;
    Deck deck_;
    Card::cardsuit trump_;
    Card trumpCard_;
};
