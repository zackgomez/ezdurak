#include "GUIPlayer.h"
#include "core/GameAgent.h"
#include <algorithm>

GUIPlayer::GUIPlayer(const std::string &name, SynchronizedQueue<int> &q) :
    Player(name),
    queue_(q),
    agent_(NULL)
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

void GUIPlayer::gameStarting(GameAgent *agent)
{
    agent_ = agent;
    sortHand();
}

Card GUIPlayer::defend(const Card& attackingCard, Card::cardsuit trump)
{ queue_.clear();

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
        if (cnum == -1 && !playableRanks.empty())
            return Card();

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
    Player::addCards(cards);
    if (agent_)
        sortHand();
}

class CardComp
{
private:
    Card::cardsuit trump;
public:
    CardComp(Card::cardsuit trumpsuit) :
        trump(trumpsuit)
    {}

    bool operator()(const Card &a, const Card &b)
    {
        Card::cardsuit aSuit = a.getSuit();
        Card::cardsuit bSuit = b.getSuit();

        // Case 1: one trump, one not trump
        if ((aSuit == trump && bSuit != trump) || 
            (bSuit == trump && aSuit != trump))
        {
            return a.getSuit() != trump;
        }
        // Case 2: both trump or both not trump
        return a.getNum() < b.getNum();
    }
};

void GUIPlayer::sortHand()
{
    Card::cardsuit trump = agent_->getTrumpCard().getSuit();
    sort(hand_.begin(), hand_.end(), CardComp(trump));
}
