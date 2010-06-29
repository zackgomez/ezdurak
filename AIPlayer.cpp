#include "AIPlayer.h"
#include <vector>
#include <set>
#include <cassert>
#include <iostream>
#include "GameAgent.h"

using std::cout;
using std::vector;
using std::set;
using std::string;

AIPlayer::AIPlayer(GameAgent* agent, const string& name,
                   const vector<Card>& hand) :
    Player(name, hand),
    GameListener(agent)
{ /* Empty */ }

AIPlayer::~AIPlayer()
{ /* Empty */ }

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


void AIPlayer::attackerChanged(const Player *newAttacker)
{ /* Empty */ }

void AIPlayer::defenderChanged(const Player *newDefender)
{ /* Empty */ }

void AIPlayer::defenderLost()
{ /* Empty */ }

void AIPlayer::defenderWon()
{ /* Empty */ }

void AIPlayer::attackingCard(const Card &c)
{ /* Empty */ }

void AIPlayer::defendingCard(const Card &c)
{ /* Empty */ }

void AIPlayer::playedOut(const Player *player)
{ /* Empty */ }

void AIPlayer::givenCards(const Player *player, int numCards)
{ /* Empty */ }

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
