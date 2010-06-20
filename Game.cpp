#include "Game.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cassert>
#include "GamePlayer.h"

using namespace std;

Game::Game(int numPlayers) :
    players_(numPlayers),
    attackerIdx_(0),
    defenderIdx_(1)
{
    assert(numPlayers >= 2 && numPlayers <= 6);

    deck_.shuffle();
    trumpCard_ = deck_.peekLast();
    trump_ = trumpCard_.getSuit();

    for (int i = 0; i < players_.size(); i++)
    {
        stringstream ss;
        ss << "Player" << i;
        string name = ss.str();
        vector<Card> hand = deck_.deal(6);
        players_[i] = new GamePlayer(name, hand);
    }
}

Game::~Game()
{
    for (int i = 0; i < players_.size(); i++)
        delete players_[i];
}

void Game::run()
{
    cout << "Starting a new game with " << players_.size() << " players\n";
    cout << trumpCard_ << " is trump\n";

    for (;;)
    {
        bool successfulDefend = doRound();

        refillCards();

        if (successfulDefend)
            attackerIdx_ = defenderIdx_;
        else
            attackerIdx_ = (defenderIdx_+1) % players_.size();
        defenderIdx_ = (attackerIdx_+1) % players_.size();
    }

}

bool Game::doRound()
{
    GamePlayer *attacker = players_[attackerIdx_];
    GamePlayer *defender = players_[defenderIdx_];
    set<int> playedRanks;
    std::vector<Card> playedCards;
    int giveUps = 0;
    int nextAttackerIdx = (defenderIdx_+1) % players_.size();
    // Maximum of 6, or the number of cards the player had cards to be played
    int maxCards = min(6, defender->getNumCards());
    for (int i = 0; i < maxCards; i++)
    {
        cout << attacker->getName() << " is attacking " <<
            defender->getName() << '\n';

        Card attC = attacker->attack(playedRanks);
        if (attC == Card())
        {
            // If all the attackers have passed, successful defend
            giveUps++;
            if (giveUps == players_.size() - 1)
                return true;
            // Make sure the defender doesn't attack themselves
            if (nextAttackerIdx == defenderIdx_)
                nextAttackerIdx = (nextAttackerIdx+1) % players_.size();
            attacker = players_[nextAttackerIdx];
            nextAttackerIdx = (nextAttackerIdx+1) % players_.size();
            // Next attacker's turn
            continue;
        }
        // If one attacker plays a card reset the giveUp count
        giveUps = 0;
        playedCards.push_back(attC);

        Card defC = defender->defend(attC, trump_);
        if (defC == Card())
        {
            cout << "Giving the defender " << playedCards.size() << " cards\n";
            defender->addCards(playedCards);
            return false;
        }
        playedCards.push_back(defC);

        // Add the cards to the played ranks
        playedRanks.insert(attC.getNum());
        playedRanks.insert(defC.getNum());
    }

    return true;
}

void Game::refillCards()
{
    // TODO smart refill
    for (int i = 0; i < players_.size(); i++) 
    {
        int numCards = min(deck_.getNumCards(), 6 - players_[i]->getNumCards());
        if (numCards > 0)
        {
            vector<Card> newcs = deck_.deal(numCards);
            players_[i]->addCards(newcs);
        }
    }
}
