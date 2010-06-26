#include "Game.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cassert>
#include "Player.h"
#include "CLIPlayer.h"
#include "GameListener.h"

using namespace std;

Game::Game(int numPlayers) :
    players_(numPlayers),
    attackerIdx_(0),
    defenderIdx_(1)
{
    assert(numPlayers >= 2 && numPlayers <= 6);

    // Predeal the hands so that we can ensure that the game will be fair 
    // without unnecessarily making and remaking players
    std::vector<std::vector<Card>> hands(numPlayers);
    for (;;)
    {
        // Shuffle and get trump card
        deck_ = Deck();
        deck_.shuffle();
        trumpCard_ = deck_.peekLast();
        trump_ = trumpCard_.getSuit();

        // Deal and validate hands
        for (int i = 0; i < numPlayers; i++)
        {
            hands[i] = deck_.deal(6);
            if (!validateHand(hands[i]))
            {
                cout << "Misdeal!\n";
                continue;
            }
        }

        break;
    }


    for (int i = 0; i < players_.size(); i++)
    {
        stringstream ss;
        ss << "Player" << i;
        string name = ss.str();
        players_[i] = new CLIPlayer(name, hands[i]);
    }

}

Game::~Game()
{
    for (int i = 0; i < players_.size(); i++)
        delete players_[i];
}

// -------------- Functions Inherited from GameAgent Interface ----------------

void Game::addListener(GameListener *listener)
{
    listeners_.insert(listener);
}

void Game::removeListener(GameListener *listener)
{
    listeners_.erase(listener);
}

Card::cardsuit Game::getTrump() const
{
    return trump_;
}

const Player * Game::getAttacker() const
{
    return attacker_;
}

const Player * Game::getDefender() const
{
    return defender_;
}

const std::vector<Player*>& Game::getPlayers() const
{
    return players_;
}

const std::vector<Card>& Game::getPlayedCards() const
{
    return playedCards_;
}

// -------------- Interface Functions -----------------
void Game::run()
{
    cout << "Starting a new game with " << players_.size() << " players\n";
    cout << trumpCard_ << " is trump\n";

    while (players_.size() > 1)
    {
        playedCards_.clear();
        playedRanks_.clear();
        attacker_ = players_[attackerIdx_];
        defender_ = players_[defenderIdx_];
        nextAttackerIdx_ = (defenderIdx_+1) % players_.size();

        bool successfulDefend = doRound();

        refillCards();
        
        // Check for players with no cards and remove them from the players list
        for (std::vector<Player*>::iterator it = players_.begin(); it != players_.end(); it++)
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
    int giveUps = 0;
    // Maximum of 6, or the number of cards the player had cards to be played
    int maxCards = min(6, defender_->getNumCards());
    for (int i = 0; i < maxCards; i++)
    {
        cout << attacker_->getName() << " is attacking " <<
            defender_->getName() << '\n';

        Card attC = attacker_->attack(playedRanks_);
        if (!attC)
        {
            // If all the attackers have passed, successful defend
            giveUps++;
            if (giveUps == players_.size() - 1)
                break;
            
            nextAttacker();

            // No card played
            i--;
            continue;
        }
        // If one attacker plays a card reset the giveUp count
        giveUps = 0;
        playedCards_.push_back(attC);
        // TODO Broadcast card

        Card defC = defender_->defend(attC, trump_);
        if (!defC)
        {
            cout << "The defender gave up\n";
            // Give the defender all of the cards
            pileOn(maxCards - i);
            cout << "Giving the defender " << playedCards_.size() << " cards\n";
            defender_->addCards(playedCards_);
            // Unsuccessful defend
            // TODO Broadcast to listeners
            return false;
        }
        playedCards_.push_back(defC);
        // TODO broadcast card
        cout << "The defending card is " << defC << '\n';

        // Add the cards to the played ranks
        playedRanks_.insert(attC.getNum());
        playedRanks_.insert(defC.getNum());
    }

    // If the max number of cards has been played, successful defend
    // TODO broadcast to listeners
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

    // Broadcast to the listeners
    for (auto it = listeners_.begin(); it != listeners_.end(); it++)
        (*it)->attackerChanged();
}

void Game::pileOn(int maxCards)
{
    int passers = 0;
    while (maxCards > 0)
    {
        Card c = attacker_->pileOn(playedRanks_);
        if (!c)
        {
            passers++;
            if (passers == players_.size() - 1)
                return;

            nextAttacker();
            continue;
        }
        playedCards_.push_back(c);
        // TODO broadcast card
        maxCards--;
        passers = 0;
        // No need to update played ranks, no new values will be played.
    }
}

bool Game::validateHand(const vector<Card>& hand)
{
    assert(hand.size() == 6);

    int hcount, scount, ccount, dcount;
    hcount = scount = ccount = dcount = 0;
    int black = 0;
    int red = 0;

    for (int i = 0; i < hand.size(); i++)
    {
        if (hand[i].getSuit() == Card::hearts)
        {
            red++;
            hcount++;
        }
        else if (hand[i].getSuit() == Card::diamonds)
        {
            red++;
            dcount++;
        }
        else if (hand[i].getSuit() == Card::clubs)
        {
            black++;
            ccount++;
        }
        else if (hand[i].getSuit() == Card::spades)
        {
            black++;
            scount++;
        }
    }

    return red != 6 && black != 6 &&
        hcount < 5 && scount < 5 && ccount < 5 && dcount < 5;
}
