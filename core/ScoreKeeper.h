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
    void gameStart();
    void gameOver(const Player* biscuit);

    // Ignored messages
    void newRound(const Player*, const Player*);
    void attackerPassed(const Player*);
    void endRound(bool);
    void attackingCard(const Card &);
    void defendingCard(const Card &);
    void piledOnCard(const Card &);
    void playedOut(const Player *);
    void givenCards(const Player *, int);

private:
    std::map<std::string, int> scores_;
    int numDraws_;
};

