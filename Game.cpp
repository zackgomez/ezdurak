#include "Game.h"
#include <cassert>
#include <algorithm>
#include "Player.h"
#include "GameListener.h"

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
    // First thing, deal out the hands
    deal();

    // The game begins
    for (auto it = players_.begin(); it != players_.end(); it++)
        (*it)->gameStarting(this);
    for (auto it = listeners_.begin(); it != listeners_.end(); it++)
        (*it)->gameStart();

    // The game continues while there is more than 1 player
    while (players_.size() > 1)
    {
        // Broadcast start of a round
        for (auto it = listeners_.begin(); it != listeners_.end(); it++)
            (*it)->newRound(attacker_, defender_);
        // Round
        bool successfulDefend = doRound();
        // Broadcast end of round
        for (auto it = listeners_.begin(); it != listeners_.end(); it++)
            (*it)->endRound(successfulDefend);
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

    const Player* biscuit = NULL;
    // If there is a player left, they're the biscuit
    if (players_.size() == 1)
        biscuit = players_[0];
    // Broadcast the game over with the loser or NULL
    for (auto it = listeners_.begin(); it != listeners_.end(); it++)
        (*it)->gameOver(biscuit);
}

void Game::deal()
{
    vector<vector<Card> > hands(players_.size());
    // Loop invariant: the hands are not valid
    do
    {
        deck_ = Deck();
        deck_.shuffle();
        // Bottom card is trump
        trumpCard_ = deck_.peekLast();

        // Deal the hands
        for (int i = 0; i < players_.size(); i++)
            hands[i] = deck_.deal(HAND_SIZE);
    } while (!validateHands(hands));

    // Give each player their hand
    for (int i = 0; i < players_.size(); i++)
        players_[i]->addCards(hands[i]);
}

bool Game::validateHands(const vector<vector<Card> >& hands) const
{
    // Some sanity checks
    assert(hands.size() == players_.size());

    // Loop over each hand and see if its no good
    for (int i = 0; i < hands.size(); i++)
    {
        assert(hands[i].size() == HAND_SIZE);
        int red = 0, black = 0;
        int ccnt = 0, scnt = 0, hcnt = 0, dcnt = 0;
        // Loop over each card in the hand and update the count variables
        for (int j = 0; j < hands[i].size(); j++)
        {
            Card::cardsuit s = hands[i][j].getSuit();
            red   += (s == Card::hearts || s == Card::diamonds) ? 1 : 0;
            black += (s == Card::clubs || s == Card::spades) ? 1 : 0;
            ccnt  += (s == Card::clubs) ? 1 : 0;
            scnt  += (s == Card::spades) ? 1 : 0;
            hcnt  += (s == Card::hearts) ? 1 : 0;
            dcnt  += (s == Card::diamonds) ? 1 : 0;
        }

        // If you have all one color, or more than 4 of one suit, it's a
        // misdeal
        if (red == HAND_SIZE || black == HAND_SIZE ||
            ccnt > 4 || scnt > 4 || hcnt > 4 || dcnt > 4)
            return false;
    }

    // If there are no problem hands, then they are all valid
    return true;
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

    // Loop invariant: There are still attacking cards to be played
    while (tricksLeft_ > 0)
    {
        // Get the next attacking card from any attacker
        Card attC = getAttackingCard();
        // Did they all pass?
        if (!attC)
            // If so, the defender won!
            return true;
        
        // Record the card
        playedCards_.push_back(attC);
        playableRanks_.insert(attC.getNum());
        --tricksLeft_;
        // Broadcast the card
        for (auto it = listeners_.begin(); it != listeners_.end(); it++)
            (*it)->attackingCard(attC);

        // Get the card from the defender
        Card defC = defender_->defend(attC, trumpCard_.getSuit());
        // If the defender gives up
        if (!defC)
            // Then they lose!
            return false;

        // Record the card
        playedCards_.push_back(defC);
        playableRanks_.insert(defC.getNum());
        // Broadcast the card
        for (auto it = listeners_.begin(); it != listeners_.end(); it++)
            (*it)->defendingCard(defC);
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
    // loop invariant: there are cards left to play
    while (tricksLeft_ > 0)
    {
        Card attC = getAttackingCard();
        // Did the play a card?
        if (attC)
        {
            // Record it
            playedCards_.push_back(attC);
            --tricksLeft_;
            // Broadcast it
            for (auto it = listeners_.begin(); it != listeners_.end(); it++)
                (*it)->piledOnCard(attC);
        }
        // Did they all pass?
        else
            // Then we're done
            break;
    }
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
            const Player *goingOut = players_[i];
            // Remove that player using random access iterator
            // The next player is now accessed using the same index i
            players_.erase(players_.begin() + i);
            // Broadcast the player
            for (auto it = listeners_.begin(); it != listeners_.end(); it++)
                (*it)->playedOut(goingOut);
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
