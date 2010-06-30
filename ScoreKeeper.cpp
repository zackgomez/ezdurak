#include "ScoreKeeper.h"
#include <iostream>
#include "Player.h"

ScoreKeeper::ScoreKeeper()
{}

ScoreKeeper::~ScoreKeeper()
{}

void ScoreKeeper::print()
{
    for (auto it = scores_.begin(); it != scores_.end(); it++)
    {
        std::cout << (*it).first << " had " << (*it).second << " losses\n";
    }
}

void ScoreKeeper::gameStart()
{ /* Empty */ }

void ScoreKeeper::gameOver(const Player* biscuit)
{
    scores_[biscuit->getName()]++;
}

void ScoreKeeper::attackerChanged(const Player *)
{ /* Empty */ }

void ScoreKeeper::defenderChanged(const Player *)
{ /* Empty */ }

void ScoreKeeper::defenderLost()
{ /* Empty */ }

void ScoreKeeper::defenderWon()
{ /* Empty */ }

void ScoreKeeper::attackingCard(const Card &)
{ /* Empty */ }

void ScoreKeeper::defendingCard(const Card &)
{ /* Empty */ }

void ScoreKeeper::playedOut(const Player *)
{ /* Empty */ }

void ScoreKeeper::givenCards(const Player *, int)
{ /* Empty */ }
