#include "GUIListener.h"
#include <SDL/SDL.h>
#include <vector>
#include "GUIImpl.h"

using std::vector;

GUIListener::GUIListener(GameAgent *agent, GUIImpl *impl)
    : GameListener(),
    impl_(impl)
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
{
    vector<Card> playedCards = agent_->getPlayedCards();
    impl_->setPlayedCards(playedCards);
}

void GUIListener::attackerPassed(const Player *newAttacker)
{ /* Empty */ }

void GUIListener::endRound(bool successfulDefend)
{ /* Empty */ }

void GUIListener::attackingCard(const Card &c)
{
    vector<Card> playedCards = agent_->getPlayedCards();
    impl_->setPlayedCards(playedCards);
}

void GUIListener::defendingCard(const Card &c)
{
    vector<Card> playedCards = agent_->getPlayedCards();
    impl_->setPlayedCards(playedCards);
}

void GUIListener::piledOnCard(const Card &c)
{
    vector<Card> playedCards = agent_->getPlayedCards();
    impl_->setPlayedCards(playedCards);
}

void GUIListener::playedOut(const Player *player)
{ /* Empty */ }

void GUIListener::givenCards(const Player *player, int numCards)
{ /* Empty */ }
