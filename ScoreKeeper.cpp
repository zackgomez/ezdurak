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
    for (auto it = scores_.begin(); it != scores_.end(); it++)
    {
        std::cout << (*it).first << " had " << (*it).second << " losses\n";
    }
}

void ScoreKeeper::gameStart()
{ /* Empty */ }

void ScoreKeeper::gameOver(const Player* biscuit)
{
    if (biscuit)
        scores_[biscuit->getName()]++;
    else
        ++numDraws_;
}

void ScoreKeeper::newRound(const Player *, const Player*)
{ /* Empty */ }

void ScoreKeeper::attackerPassed(const Player *)
{ /* Empty */ }

void ScoreKeeper::endRound(bool)
{ /* Empty */ }

void ScoreKeeper::attackingCard(const Card &)
{ /* Empty */ }

void ScoreKeeper::defendingCard(const Card &)
{ /* Empty */ }

void ScoreKeeper::piledOnCard(const Card &)
{ /* Empty */ }

void ScoreKeeper::playedOut(const Player *)
{ /* Empty */ }

void ScoreKeeper::givenCards(const Player *, int)
{ /* Empty */ }
