#include "CardCounter.h"
#include "core/GameAgent.h"

CardCounter::CardCounter()
{
}

CardCounter::~CardCounter()
{
}

bool CardCounter::isDiscarded(const Card& c) const
{
    return discardedCards_.find(c) != discardedCards_.end();
}

const CardCounter::cardset& CardCounter::discardedCards() const
{
    return discardedCards_;
}

void CardCounter::gameStart(GameAgent *agent)
{
    agent_ = agent;

    std::vector<PlayerPtr> players = agent_->getPlayers();
    for (int i = 0; i < players.size(); i++)
    {
        playerCards_[players[i]] = cardset();
    }
}

void CardCounter::gameOver(const ConstPlayerPtr biscuit)
{
    // Ignored
}
    
void CardCounter::newRound(const ConstPlayerPtr attacker, const ConstPlayerPtr defender)
{
    attacker_ = attacker;
    defender_ = defender;
    playedCards_.clear();
}
    
void CardCounter::attackerPassed(const ConstPlayerPtr newAttacker)
{
    attacker_ = newAttacker;
}
    
void CardCounter::endRound(bool successful)
{
    if (successful)
        discardedCards_.insert(playedCards_.begin(), playedCards_.end());
}
    
void CardCounter::attackingCard(const Card &c)
{
    playerCards_[attacker_].erase(c);
}
    
void CardCounter::defendingCard(const Card &c)
{
    playerCards_[defender_].erase(c);
}
    
void CardCounter::piledOnCard(const Card &c)
{
    playerCards_[attacker_].erase(c);
}
    
void CardCounter::playedOut(ConstPlayerPtr p)
{
    // Ignored
}
    
void CardCounter::givenCards(ConstPlayerPtr p, int n)
{
    // If there are no cards left,they took the trump card at the bottom
    if (agent_->getDeckSize() == 0)
        playerCards_[p].insert(agent_->getTrumpCard());
}
    
void CardCounter::givenCards(ConstPlayerPtr player, const std::vector<Card>& cs)
{
    playerCards_[player].insert(cs.begin(), cs.end());
}
