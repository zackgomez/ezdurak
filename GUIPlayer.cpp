#include "GUIPlayer.h"
#include <iostream>

GUIPlayer::GUIPlayer(const std::string &name, SynchronizedQueue<int> &q) :
    CLIPlayer(name),
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
