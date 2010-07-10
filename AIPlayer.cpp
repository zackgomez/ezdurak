#include "AIPlayer.h"
#include <vector>
#include <set>
#include <cassert>
#include <iostream>
#include <cstdlib>

using std::cout;
using std::vector;
using std::set;
using std::string;

AIPlayer::AIPlayer(const string& name) :
    Player(name)
{ /* Empty */ }

AIPlayer::~AIPlayer()
{ /* Empty */ }

void AIPlayer::gameStarting(GameAgent *agent)
{
}

Card AIPlayer::defend(const Card& attackingCard, Card::cardsuit trump)
{
    vector<Card> playable = defendableCards(attackingCard, trump);

    if (playable.size() == 0)
        return Card();

    int idx = (rand() % playable.size());
    Card defC = playable[idx];

    removeCard(defC);

    return defC;
}

Card AIPlayer::attack(set<int> playableRanks)
{
    vector<Card> playable = playableCards(playableRanks);

    if (playable.size() == 0)
        return Card();

    int idx = (rand() % playable.size());
    Card attC = playable[idx];

    removeCard(attC);

    return attC;
}

Card AIPlayer::pileOn(set<int> playableRanks)
{
    return attack(playableRanks);
}

void AIPlayer::addCards(const vector<Card>& cards)
{
    Player::addCards(cards);
}

vector<Card> AIPlayer::playableCards(set<int> playableRanks)
{
    if (playableRanks.size() == 0)
        return hand_;

    vector<Card> playable;
    for (int i = 0; i < hand_.size(); i++)
    {
        if (playableRanks.find(hand_[i].getNum()) != playableRanks.end())
            playable.push_back(hand_[i]);
    }

    return playable;
}

vector<Card> AIPlayer::defendableCards(const Card& attackingCard,
                                       Card::cardsuit trump)
{
    vector<Card> playable;

    for (int i = 0; i < hand_.size(); i++)
    {
        if (hand_[i].beats(attackingCard, trump))
            playable.push_back(hand_[i]);
    }

    return playable;
}

void AIPlayer::removeCard(const Card& card)
{
    for (int i = 0; i < hand_.size(); i++)
    {
        if (hand_[i] == card)
        {
            hand_[i] = hand_[hand_.size() - 1];
            hand_.pop_back();
            return;
        }
    }

    assert(false && "Tried to remove a card that wasn't in the hand");
}
