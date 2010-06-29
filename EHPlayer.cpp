/* EHPlayer.cpp */

#include "EHPlayer.h"
#include <iostream>
#include <vector>
#include <set>
#include <string>

using std::cout;
using std::vector;
using std::set;
using std::string;

EHPlayer::EHPlayer(GameAgent* agent, const string& name,
                   const vector<Card>& hand) :
    AIPlayer(agent, name, hand)
{ /* Empty */ }

EHPlayer::~EHPlayer()
{ /* Empty */ }

Card EHPlayer::attack(set<int> playableRanks)
{
    vector<Card> playable = playableCards(playableRanks);

    if (playable.size() == 0)
        return Card();

    Card::cardsuit trumpSuit = agent_->getTrumpCard().getSuit();

    int idx = rand() % playable.size();
    Card attC = playable[idx];

    for (int i = 0; i < playable.size(); i++)
    {
        if (playable[i].getSuit() != trumpSuit)
        {
            attC = playable[i];
            break;
        }
    }
    removeCard(attC);
    return attC;
}
