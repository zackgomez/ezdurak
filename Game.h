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
    const static int HAND_SIZE = 6;

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
    std::vector<Card> playedCards_;

    std::set<GameListener*> listeners_;

    void deal();
    void doRound();
};

