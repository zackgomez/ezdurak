#include "AIPlayer.h"
#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include "core/GameAgent.h"

using std::cout;
using std::vector;
using std::set;
using std::string;
using std::sort;

AIPlayer::AIPlayer(const string& name) :
    PlayerImpl(name)
{ /* Empty */ }

AIPlayer::~AIPlayer()
{ /* Empty */ }

void AIPlayer::gameStarting(GameAgent *agent)
{
    agent_ = agent;
}

Card AIPlayer::defend(const Card& attackingCard, Card::cardsuit trump)
{
    vector<Card> playable = defendableCards(attackingCard, trump);

    // If we have no cards that will defend it, we have to give up
    if (playable.size() == 0)
        return Card();

    // Sort them in order of goodness
    orderCards(playable);

    // Use the lowest to defend, note, at least one card in playable
    Card defC = playable[0];

    removeCard(defC);

    return defC;
}

Card AIPlayer::attack(set<int> playableRanks)
{
    // Don't attack our partner, unless we have to
    if (isPartner(agent_->getDefender()) && !playableRanks.empty())
        return Card();

    vector<Card> playable = playableCards(playableRanks);

    // If we have no cards that are playable, we must pass
    if (playable.size() == 0)
        return Card();

    // Order tha cards..
    orderCards(playable);
    Card attC = playable[0];

    removeCard(attC);

    return attC;
}

Card AIPlayer::pileOn(set<int> playableRanks)
{
    vector<Card> playable = playableCards(playableRanks);

    // If we have no cards that are playable, we must pass
    if (playable.size() == 0)
        return Card();

    // Order tha cards..
    orderCards(playable);
    Card attC = playable[0];

    // Don't pile on trump
    if (attC.getSuit() == agent_->getTrumpCard().getSuit())
        return Card();

    // Otherwise pile it on
    removeCard(attC);
    return attC;
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

vector<Card> AIPlayer::playableCards(set<int> playableRanks) const
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
                                       Card::cardsuit trump) const
{
    vector<Card> playable;

    for (int i = 0; i < hand_.size(); i++)
    {
        if (hand_[i].beats(attackingCard, trump))
            playable.push_back(hand_[i]);
    }

    return playable;
}

void AIPlayer::orderCards(std::vector<Card>& cards) const
{
    Card::cardsuit trump = agent_->getTrumpCard().getSuit();
    sort(cards.begin(), cards.end(), CardComp(trump));
}

bool AIPlayer::isPartner(ConstPlayerPtr p) const
{
    const std::vector<PlayerPtr> players = agent_->getPlayers();

    int thisIdx = -1;
    int pIdx = -1;
    for (int i = 0; i < players.size(); i++)
    {
        if (players[i].get() == this)
            thisIdx = i;
        if (players[i] == p)
            pIdx = i;
    }

    return ((pIdx + 2) % players.size()) == thisIdx;
}
