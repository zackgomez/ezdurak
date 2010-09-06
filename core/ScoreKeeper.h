#pragma once
#include <map>
#include "GameListener.h"

/**
 * A utility class that listeners for the gameStart and gameOver messages of
 * the GameListener interface and records, over a number of games, the 
 * cumulative win/loss for players participating in the games.
 */
class ScoreKeeper :
    public GameListener
{
public:
    ScoreKeeper();
    ~ScoreKeeper();

    void print();

    // Caught messages from GameListener interface
    void gameStart(GameAgent *agent);
    void gameOver(ConstPlayerPtr biscuit);

    // Ignored messages
    void newRound(ConstPlayerPtr, ConstPlayerPtr);
    void attackerPassed(ConstPlayerPtr);
    void endRound(bool);
    void attackingCard(const Card &);
    void defendingCard(const Card &);
    void piledOnCard(const Card &);
    void playedOut(ConstPlayerPtr);
    void givenCards(ConstPlayerPtr, int);
    void givenCards(ConstPlayerPtr, const std::vector<Card>&);

private:
    std::map<std::string, int> scores_;
    int numDraws_;
};

