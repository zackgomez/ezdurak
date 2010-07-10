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
{
    const vector<Player*> players = agent_->getPlayers();
    impl_->setPlayers(players);
}

void GUIListener::gameOver(const Player *biscuit)
{ /* Empty */ }

void GUIListener::newRound(const Player *attacker, const Player *defender)
{
    impl_->clearPlayedCards();
    impl_->setPileSizes(agent_->getDeckSize(), agent_->getDiscardSize());
}

void GUIListener::attackerPassed(const Player *newAttacker)
{ /* Empty */ }

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

void GUIListener::playedOut(const Player *player)
{ /* Empty */ }

void GUIListener::givenCards(const Player *player, int numCards)
{ /* Empty */ }
