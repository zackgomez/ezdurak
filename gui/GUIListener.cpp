#include "GUIListener.h"
#include <SDL/SDL.h>
#include <vector>
#include "GUIImpl.h"
#include "core/GameAgent.h"

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
{
    const vector<PlayerPtr> players = agent_->getPlayers();
    impl_->setPlayers(players);
    impl_->setTrumpCard(agent_->getTrumpCard());
}

void GUIListener::gameOver(ConstPlayerPtr biscuit)
{
    impl_->setBiscuit(biscuit);
    impl_->setAttacker(PlayerPtr());
    impl_->setDefender(PlayerPtr());
}

void GUIListener::newRound(ConstPlayerPtr attacker, ConstPlayerPtr defender)
{
    impl_->clearPlayedCards();
    impl_->setPileSizes(agent_->getDeckSize(), agent_->getDiscardSize());
    impl_->setAttacker(attacker);
    impl_->setDefender(defender);
}

void GUIListener::attackerPassed(ConstPlayerPtr newAttacker)
{
    impl_->setAttacker(newAttacker);
}

void GUIListener::endRound(bool successfulDefend)
{ /* Empty */ }

void GUIListener::attackingCard(const Card &c)
{
    impl_->addAttackingCard(c);

    impl_->wait(400);
}

void GUIListener::defendingCard(const Card &c)
{
    impl_->addDefendingCard(c);

    impl_->wait(400);
}

void GUIListener::piledOnCard(const Card &c)
{
    impl_->addAttackingCard(c);

    impl_->wait(400);
}

void GUIListener::playedOut(ConstPlayerPtr player)
{ /* Empty */ }

void GUIListener::givenCards(ConstPlayerPtr player, int numCards)
{ /* Empty */ }
