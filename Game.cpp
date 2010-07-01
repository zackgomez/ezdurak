#include "Game.h"
#include <cassert>
#include <algorithm>
#include "Player.h"

#include <iostream>

using namespace std;

const int Game::HAND_SIZE = 6;

Game::Game(const vector<Player*>& players) :
    players_(players)
{
    assert(players_.size() >= 2 && players_.size() <= 6);

    attacker_ = players_[0];
    attackerIdx_ = 0;
    defender_ = players_[1];
}

Game::~Game()
{ /* Empty */ }

void Game::run()
{
    cout << "Beginning a game with: " << players_.size() << " players\n";
    deal();

    while (players_.size() > 1)
    {
        cout << "---------------------------------------------------\n"
             << "Start of a new round\n"
             << "Attacker: " << attacker_->getName() << '\n'
             << "Defender: " << defender_->getName() << "\n\n";
        // Round
        bool successfulDefend = doRound();
        // If the defender loses, then they must take all of the played cards.
        if (!successfulDefend)
        {
            // And the attackers can pile on cards
            pileOn();
            defender_->addCards(playedCards_);
        }
        // Refill
        refill();
        // Go out
        removeFinishedPlayers();
        // new defender
        nextDefender(successfulDefend);
    }

    if (players_.size() == 0)
        cout << "The game is a draw.\n";
    else
        cout << "The biscuit is " << players_[0]->getName() << '\n';
}

void Game::deal()
{
    // TODO:2010-06-30:zack: Check for misdeals
    deck_ = Deck();
    deck_.shuffle();
    // Bottom card is trump
    trumpCard_ = deck_.peekLast();

    // Deal the hands
    vector<vector<Card>> hands(players_.size());
    for (int i = 0; i < players_.size(); i++)
        hands[i] = deck_.deal(HAND_SIZE);

    // Give each player their hand
    for (int i = 0; i < players_.size(); i++)
        players_[i]->addCards(hands[i]);
}

// TODO:2010-06-30:zack: Error checking on played cards.
// Make sure that the first round the attacker plays a card, and that attC is
// a playable card, and that defC beats attC.
bool Game::doRound()
{
    tricksLeft_ = min(defender_->getNumCards(), HAND_SIZE);
    assert(tricksLeft_ > 0); // Sanity Check, defender must have at least 1 card
    playedCards_.clear();
    playableRanks_.clear();

    while (tricksLeft_ > 0)
    {
        // Get the next attacking card from any attacker
        Card attC = getAttackingCard();
        // Did they all pass?
        if (!attC)
            // If so, the defender won!
            return true;
        
        // Record the card
        cout << "Attacking card: " << attC << '\n';
        playedCards_.push_back(attC);
        playableRanks_.insert(attC.getNum());
        --tricksLeft_;

        // Get the card from the defender
        Card defC = defender_->defend(attC, trumpCard_.getSuit());
        // If the defender gives up
        if (!defC)
            // Then they lose!
            return false;

        // Record the card
        cout << "Defending card: " << defC << '\n';
        playedCards_.push_back(defC);
        playableRanks_.insert(defC.getNum());
    }

    // If there are no tricks left to play, then the defender has won!
    return true;
}

Card Game::getAttackingCard()
{
    // Used to tell when all the attackers have passed
    const Player *initialAttacker = attacker_;

    // while all the attackers haven't passed
    do
    {
        cout << "Current attacker:\n";
        attacker_->print();
        // Get the card from the attacker, could be a pass
        Card attC = attacker_->attack(playableRanks_);

        // If they didn't pass
        if (attC)
            // Then they played a card!
            return attC;
        // If they did pass...
        nextAttacker();
    } while (attacker_ != initialAttacker);

    return Card();
}

void Game::nextAttacker()
{
    // loop invariant: the attacker is the current defender
    do
    {
        // Make sure to wrap around
        attackerIdx_ = (attackerIdx_ + 1) % players_.size();
        attacker_ = players_[attackerIdx_];
    } while (attacker_ == defender_);
}

void Game::pileOn()
{
    // TODO
}

// TODO:2010-06-30:zack: Smart refill.
// The game should give players cards in the order that they attacked, with
// the defender always last.
void Game::refill()
{
    for (int i = 0; i < players_.size(); i++)
    {
        // You refill to HAND_SIZE, but only if there are enough cards.
        int neededCards = min(HAND_SIZE - players_[i]->getNumCards(),
                              deck_.getNumCards());
        if (neededCards > 0)
        {
            vector<Card> refillCards = deck_.deal(neededCards);
            players_[i]->addCards(refillCards);
        }
    }
}

void Game::removeFinishedPlayers()
{
    int i = 0;
    while (i < players_.size())
    {
        if (players_[i]->getNumCards() == 0)
        {
            cout << players_[i]->getName() << " has gone out!\n";
            // Remove that player using random access iterator
            // The next player is now accessed using the same index i
            players_.erase(players_.begin() + i);
        }
        else
        {
            i++;
        }
    }
}

void Game::nextDefender(bool successfulDefend)
{
    // If is one or less player there is no next defender
    if (players_.size() <= 1)
        return;

    // If there is a successful defend, the current defender is the new attacker
    attacker_ = defender_;
    // Set the attackerIdx_ variable
    attackerIdx_ = find(players_.begin(), players_.end(), attacker_)
        - players_.begin();
    // If not, then the current attacker is the "nextAttacker" from the point 
    // of view of the defender.
    if (!successfulDefend)
        nextAttacker();

    // Now we have the correct attacker set, the defender is always the person
    // after the initial attacker.
    int defenderIdx = (attackerIdx_ + 1) % players_.size();
    defender_ = players_[defenderIdx];
}

// ------------------ GameAgent Interface -------------------------
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

const vector<Player*>& Game::getPlayers() const
{
    return players_;
}

const vector<Card>& Game::getPlayedCards() const
{
    return playedCards_;
}
