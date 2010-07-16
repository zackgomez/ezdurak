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
    void gameOver(const ConstPlayerPtr biscuit);

    // Ignored messages
    void newRound(const ConstPlayerPtr, const ConstPlayerPtr);
    void attackerPassed(const ConstPlayerPtr);
    void endRound(bool);
    void attackingCard(const Card &);
    void defendingCard(const Card &);
    void piledOnCard(const Card &);
    void playedOut(const ConstPlayerPtr);
    void givenCards(const ConstPlayerPtr, int);

private:
    std::map<std::string, int> scores_;
    int numDraws_;
};

