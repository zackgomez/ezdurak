#include "GUIListener.h"

GUIListener::GUIListener(GameAgent *agent)
    : GameListener()
{
    agent_ = agent;
    agent_->addListener(this);
}

GUIListener::~GUIListener()
{ /* Empty */ }

void GUIListener::gameStart()
{ /* Empty */ }

void GUIListener::gameOver(const Player *biscuit)
{ /* Empty */ }

void GUIListener::newRound(const Player *attacker, const Player *defender)
{ /* Empty */ }

void GUIListener::attackerPassed(const Player *newAttacker)
{ /* Empty */ }

void GUIListener::endRound(bool successfulDefend)
{ /* Empty */ }

void GUIListener::attackingCard(const Card &c)
{ /* Empty */ }

void GUIListener::defendingCard(const Card &c)
{ /* Empty */ }

void GUIListener::piledOnCard(const Card &c)
{ /* Empty */ }

void GUIListener::playedOut(const Player *player)
{ /* Empty */ }

void GUIListener::givenCards(const Player *player, int numCards)
{ /* Empty */ }
