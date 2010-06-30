#pragma once
#include <map>
#include "GameListener.h"

class ScoreKeeper :
    public GameListener
{
public:
    ScoreKeeper();
    ~ScoreKeeper();

    void print();

    // Caught messages from GameListener interface
    void gameStart(GameAgent*);
    void gameOver(const Player* biscuit);

    // Ignored messages
    void attackerChanged(const Player*);
    void defenderChanged(const Player*);
    void defenderLost();
    void defenderWon();
    void attackingCard(const Card &);
    void defendingCard(const Card &);
    void playedOut(const Player *);
    void givenCards(const Player *, int);

private:
    std::map<std::string, int> scores_;
};

