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
        // TODO if the hand is all the same color or 5 of one suit, redeal
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

    while (players_.size() > 1)
    {
        playedCards_.clear();
        attacker_ = players_[attackerIdx_];
        defender_ = players_[defenderIdx_];
        nextAttackerIdx_ = (defenderIdx_+1) % players_.size();

        bool successfulDefend = doRound();

        refillCards();
        
        // Check for players with no cards and remove them from the players list
        for (std::vector<GamePlayer*>::iterator it = players_.begin(); it != players_.end(); it++)
            if ((*it)->getNumCards() == 0)
            {
                cout << (*it)->getName() << " has gone out!!!\n";
                it = players_.erase(it);
            }

        // Update attacker and defender index, successful defend means that the
        // defender is the next attacker, otherwise skip the defender
        if (successfulDefend)
            attackerIdx_ = defenderIdx_;
        else
            attackerIdx_ = (defenderIdx_+1) % players_.size();

        // Defender is always to the right of the attacker
        defenderIdx_ = (attackerIdx_+1) % players_.size();
    }
    
    // Check for win/tie
    if (players_.size() == 1)
        cout << players_[0]->getName() << " is the biscuit!!!\n";
    else
        cout << "The game was a tie.\n";
}

bool Game::doRound()
{
    set<int> playedRanks;
    int giveUps = 0;
    // Maximum of 6, or the number of cards the player had cards to be played
    int maxCards = min(6, defender_->getNumCards());
    for (int i = 0; i < maxCards; i++)
    {
        cout << attacker_->getName() << " is attacking " <<
            defender_->getName() << '\n';

        Card attC = attacker_->attack(playedRanks);
        if (!attC)
        {
            // If all the attackers have passed, successful defend
            giveUps++;
            if (giveUps == players_.size() - 1)
                return true;
            
            nextAttacker();

            // No card played
            i--;
            continue;
        }
        // If one attacker plays a card reset the giveUp count
        giveUps = 0;
        playedCards_.push_back(attC);

        Card defC = defender_->defend(attC, trump_);
        if (!defC)
        {
            // Give the defender all of the cards
            pileOn(maxCards - i);
            cout << "Giving the defender " << playedCards_.size() << " cards\n";
            defender_->addCards(playedCards_);
            // Unsuccessful defend
            return false;
        }
        playedCards_.push_back(defC);

        // Add the cards to the played ranks
        playedRanks.insert(attC.getNum());
        playedRanks.insert(defC.getNum());
    }

    // If the max number of cards has been played, successful defend
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

void Game::nextAttacker()
{
    // Make sure the defender doesn't attack themselves
    if (nextAttackerIdx_ == defenderIdx_)
        nextAttackerIdx_ = (nextAttackerIdx_+1) % players_.size();
    attacker_ = players_[nextAttackerIdx_];
    nextAttackerIdx_ = (nextAttackerIdx_+1) % players_.size();
}

void Game::pileOn(int maxCards)
{
    // TODO give the losing defender MORE cards
}
