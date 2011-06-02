#include "GUIPlayer.h"
#include "core/GameAgent.h"
#include <algorithm>

GUIPlayer::GUIPlayer(const std::string &name, SynchronizedQueue<int> &q) :
    PlayerImpl(name),
    queue_(q),
    action_(NONE)
{
    agent_ = NULL;
}

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

GUIPlayer::Action GUIPlayer::getAction() const
{
    return action_;
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
    action_ = DEFEND;
    queue_.clear();

    int cnum;
    Card attempt;
    for (;;)
    {
        attempt = Card();
        cnum = queue_.dequeue();
        // Did they pass?
        if (cnum == -1)
            break; // attempt is Card()

        // Get the card
        attempt = hand_[cnum];
        if (attempt.beats(attackingCard, trump))
        {
            // Remove it from their hand
            hand_.erase(hand_.begin() + cnum);
            break;
        }
    }
    action_ = NONE;
    return attempt;
}

Card GUIPlayer::attack(std::set<int> playableRanks)
{
    // If we have no cards... auto pass
    if (hand_.size() == 0)
        return Card();
    
    action_ = ATTACK;

    queue_.clear();

    int cnum;
    Card attempt;
    for (;;)
    {
        attempt = Card();
        cnum = queue_.dequeue();
        // Did they pass? Can they pass?
        if (cnum == -1)
        {
            if (!playableRanks.empty())
                break; // Attempt is Card()
            // If they can't pass, get another card
            continue;
        }

        // Get the card
        attempt = hand_[cnum];
        if (playableRanks.empty() || playableRanks.find(attempt.getNum()) != playableRanks.end())
        {
            // Remove it from their hand
            hand_.erase(hand_.begin() + cnum);
            break;
        }
    }
    action_ = NONE;
    return attempt;
}

Card GUIPlayer::pileOn(std::set<int> playableRanks)
{
    return attack(playableRanks);
}

Card GUIPlayer::deflect(const Card &attC)
{
    // Dont ask them to deflect unless they can
    bool able = false;
    for (unsigned i = 0; i < hand_.size(); i++)
        if (hand_[i].getNum() == attC.getNum())
        {
            able = true;
            break;
        }
    if (!able)
        return Card();

    action_ = DEFLECT;
    queue_.clear();

    int cnum;
    Card attempt;
    for (;;)
    {
        attempt = Card();
        cnum = queue_.dequeue();
        // Did they pass?
        if (cnum == -1)
            break; // Attempt is card()

        // Get the card
        attempt = hand_[cnum];
        if (attempt.getNum() == attC.getNum())
        {
            // Remove it from their hand
            hand_.erase(hand_.begin() + cnum);
            break;
        }
    }
    action_ = NONE;
    return attempt;
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
