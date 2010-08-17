#include "GUIPlayer.h"
#include "core/GameAgent.h"
#include <algorithm>

GUIPlayer::GUIPlayer(const std::string &name, SynchronizedQueue<int> &q) :
    PlayerImpl(name),
    queue_(q)
{ /* Empty */ }

GUIPlayer::~GUIPlayer()
{ /* Empty */ }

const std::vector<Card>& GUIPlayer::getHand() const
{
    return hand_;
}

SynchronizedQueue<int>& GUIPlayer::getQueue() const
{
    return queue_;
}

const Card GUIPlayer::getTrumpCard() const
{
    if (!agent_)
        return Card();
    return agent_->getTrumpCard();
}

void GUIPlayer::gameStarting(GameAgent *agent)
{
    PlayerImpl::gameStarting(agent);
    sortHand();
}

Card GUIPlayer::defend(const Card& attackingCard, Card::cardsuit trump)
{
    queue_.clear();

    int cnum;
    Card attempt;
    for (;;)
    {
        cnum = queue_.dequeue();
        // Did they pass?
        if (cnum == -1)
            return Card();

        // Get the card
        attempt = hand_[cnum];
        if (attempt.beats(attackingCard, trump))
            break;
    }
    // Remove it from their hand
    hand_.erase(hand_.begin() + cnum);
    return attempt;
}

Card GUIPlayer::attack(std::set<int> playableRanks)
{
    // If we have no cards... auto pass
    if (hand_.size() == 0)
        return Card();

    queue_.clear();

    int cnum;
    Card attempt;
    for (;;)
    {
        cnum = queue_.dequeue();
        // Did they pass? Can they pass?
        if (cnum == -1)
        {
            if (!playableRanks.empty())
                return Card();
            // If they can't pass, get another card
            continue;
        }

        // Get the card
        attempt = hand_[cnum];
        if (playableRanks.empty() || playableRanks.find(attempt.getNum()) != playableRanks.end())
            break;
    }
    // Remove it from their hand
    hand_.erase(hand_.begin() + cnum);
    return attempt;
}

Card GUIPlayer::pileOn(std::set<int> playableRanks)
{
    return attack(playableRanks);
}

void GUIPlayer::addCards(const std::vector<Card>& cards)
{
    PlayerImpl::addCards(cards);
    if (agent_)
        sortHand();
}

void GUIPlayer::sortHand()
{
    Card::cardsuit trump = agent_->getTrumpCard().getSuit();
    sort(hand_.begin(), hand_.end(), CardComp(trump));
}
