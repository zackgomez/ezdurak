#include "Game.h"
#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>
#include "GameListener.h"
#include "Player.h"

using namespace std;

Game::Game(const std::vector<Player*> players) :
    players_(players),
    attackerIdx_(0),
    defenderIdx_(1)
{
    int numPlayers = players_.size();
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
		// Misdeal
                continue;
            }
        }

        break;
    }

    for (int i = 0; i < players_.size(); i++)
        players_[i]->addCards(hands[i]);
    attacker_ = players_[attackerIdx_];
    defender_ = players_[defenderIdx_];
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

Card Game::getTrumpCard() const
{
    return trumpCard_;
}

int Game::getTricksLeft() const
{
    return tricksLeft_;
}

int Game::getDeckSize() const
{
    return deck_.getNumCards();
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
    // The game begins!!
    for (auto it = players_.begin(); it != players_.end(); it++)
        (*it)->gameStarting(this);
    for (auto it = listeners_.begin(); it != listeners_.end(); it++)
        (*it)->gameStart();

    while (players_.size() > 1)
    {
        playedCards_.clear();
        playedRanks_.clear();
        attacker_ = players_[attackerIdx_];
        defender_ = players_[defenderIdx_];
        nextAttackerIdx_ = (defenderIdx_+1) % players_.size();
        refillOrder_.clear();

        // Maximum of 6, or the number of cards the player had cards to be played
        tricksLeft_ = min(6, defender_->getNumCards());

        for (auto it = listeners_.begin(); it != listeners_.end(); it++)
        {
            (*it)->attackerChanged(attacker_);
            (*it)->defenderChanged(defender_);
        }
        bool successfulDefend = doRound();
        // Now add the defender to the back of the refill order
        refillOrder_.push_back(defender_);
        refillCards();

        // Update attacker and defender index, successful defend means that the
        // defender is the next attacker, otherwise skip the defender
        if (successfulDefend)
            for (auto it = listeners_.begin(); it != listeners_.end(); it++)
                (*it)->defenderWon();
        else
            for (auto it = listeners_.begin(); it != listeners_.end(); it++)
                (*it)->defenderLost();

        // Defender is always to the right of the attacker
        defenderIdx_ = (attackerIdx_+1) % players_.size();

        // Check for players with no cards and remove them from the players list
        for (auto it = players_.begin(); it != players_.end(); it++)
        {
            if ((*it)->getNumCards() == 0)
            {
                Player* p = *it;
                // Remove them from active players
                it = players_.erase(it);
                // TODO:2010-06-29:zack:This fixes the segfault but is weird.
                it = players_.begin();
                // Broadcast
                for (auto lit = listeners_.begin(); lit != listeners_.end(); lit++)
                    (*lit)->playedOut(p);
            }
        }

        // TODO:2010-06-28:zack:Fix this as they could be incorrect after people go out
        // Update indicies
        if (successfulDefend)
            attackerIdx_ = defenderIdx_;
        else
            attackerIdx_ = (defenderIdx_+1) % players_.size();
        // Defender is always to the right of the attacker
        defenderIdx_ = (attackerIdx_+1) % players_.size();
    }
    
    // Check for win/tie and let the listeners know
    Player *biscuit = NULL;
    if (players_.size() == 1)
        biscuit = players_[0];

    for (auto it = listeners_.begin(); it != listeners_.end(); it++)
        (*it)->gameOver(biscuit);
}

bool Game::doRound()
{
    int giveUps = 0;
    while (tricksLeft_ > 0)
    {
        Card attC = attacker_->attack(playedRanks_);
        if (!attC)
        {
            // If all the attackers have passed, successful defend
            giveUps++;
            if (giveUps == players_.size() - 1)
                break;
            
            nextAttacker();

            // No card played
            continue;
        }
        // If one attacker plays a card reset the giveUp count
        giveUps = 0;
        playedCards_.push_back(attC);
        playedRanks_.insert(attC.getNum());
        tricksLeft_--;
        // Keep track of the order the cards are played in
        if (find(refillOrder_.begin(), refillOrder_.end(), attacker_)
                == refillOrder_.end())
            refillOrder_.push_back(attacker_);
        // Broadcast
        for (auto it = listeners_.begin(); it != listeners_.end(); it++)
            (*it)->attackingCard(attC);

        Card defC = defender_->defend(attC, trump_);
        if (!defC)
        {
            // Give the defender all of the cards + pileOn
            pileOn();
            defender_->addCards(playedCards_); // Unsuccessful defend
            for (auto it = listeners_.begin(); it != listeners_.end(); it++)
                (*it)->givenCards(defender_, playedCards_.size());
            return false;
        }
        playedCards_.push_back(defC);
        playedRanks_.insert(defC.getNum());
        // Broadcast
        for (auto it = listeners_.begin(); it != listeners_.end(); it++)
            (*it)->defendingCard(defC);

    }

    // If the max number of cards has been played, successful defend
    return true;
}

// TODO:2010-06-29:zack: Make sure this function is working correctly
void Game::refillCards()
{
    for (int i = 0; i < refillOrder_.size(); i++) 
    {
        int numCards = min(deck_.getNumCards(), 6 - refillOrder_[i]->getNumCards());
        if (numCards > 0)
        {
            vector<Card> newcs = deck_.deal(numCards);
            refillOrder_[i]->addCards(newcs);
            for (auto it = listeners_.begin(); it != listeners_.end(); it++)
                (*it)->givenCards(refillOrder_[i], numCards);
        }
    }
}

void Game::nextAttacker()
{
    // Make sure the defender doesn't attack themselves
    if (nextAttackerIdx_ == defenderIdx_) nextAttackerIdx_ = (nextAttackerIdx_+1) % players_.size(); attacker_ = players_[nextAttackerIdx_];
    nextAttackerIdx_ = (nextAttackerIdx_+1) % players_.size();
    
    // Broadcast to the listeners
    for (auto it = listeners_.begin(); it != listeners_.end(); it++)
        (*it)->attackerChanged(attacker_);
}

void Game::pileOn()
{
    int passers = 0;
    while (tricksLeft_ > 0)
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
        --tricksLeft_;
        for (auto it = listeners_.begin(); it != listeners_.end(); it++)
            (*it)->attackingCard(c);
        tricksLeft_--;
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

    return red < 6 && black < 6 &&
        hcount < 5 && scount < 5 && ccount < 5 && dcount < 5;
}
