#include "ScoreKeeper.h"
#include <iostream>
#include "Player.h"

ScoreKeeper::ScoreKeeper() :
    numDraws_(0)
{}

ScoreKeeper::~ScoreKeeper()
{}

void ScoreKeeper::print()
{
    std::cout << "There were " << numDraws_ << " draws\n";
    std::map<std::string, int>::iterator it;
    for (it = scores_.begin(); it != scores_.end(); it++)
    {
        std::cout << (*it).first << " had " << (*it).second << " losses\n";
    }
}

void ScoreKeeper::gameStart(GameAgent *)
{ /* Empty */ }

void ScoreKeeper::gameOver(const ConstPlayerPtr biscuit)
{
    if (biscuit)
        scores_[biscuit->getName()]++;
    else
        ++numDraws_;
}

void ScoreKeeper::newRound(const ConstPlayerPtr, const ConstPlayerPtr)
{ /* Empty */ }

void ScoreKeeper::attackerPassed(const ConstPlayerPtr)
{ /* Empty */ }

void ScoreKeeper::endRound(bool)
{ /* Empty */ }

void ScoreKeeper::attackingCard(const Card &)
{ /* Empty */ }

void ScoreKeeper::defendingCard(const Card &)
{ /* Empty */ }

void ScoreKeeper::piledOnCard(const Card &)
{ /* Empty */ }

void ScoreKeeper::playedOut(ConstPlayerPtr)
{ /* Empty */ }

void ScoreKeeper::givenCards(ConstPlayerPtr, int)
{ /* Empty */ }

void ScoreKeeper::givenCards(ConstPlayerPtr, const std::vector<Card>&)
{ /* Empty */ }
