#include "Game.h"
#include <cassert>
#include <algorithm>
#include "Player.h"
#include "GameListener.h"
#include <iostream>

using namespace std;

const unsigned Game::HAND_SIZE = 6;

Game::Game()
{ /* Empty */ }

Game::~Game()
{ /* Empty */ }

void Game::setPlayers()
{
    assert(players_.size() >= 2 && players_.size() <= 6);
    // Give them a good shuffling
    std::random_shuffle(players_.begin(), players_.end());

    attacker_ = players_[0];
    attackerIdx_ = 0;
    defender_ = players_[1];
    defenderIdx_ = 1;
}

void Game::addPlayer(PlayerPtr player)
{
    assert(players_.size() < 6);
    players_.push_back(player);
}

int Game::getNumPlayers() const
{
    return players_.size();
}

void Game::run()
{
    // Set up the players
    setPlayers();

    // First thing, deal out the hands
    deal();

    // The game begins
    vector<PlayerPtr>::iterator it;
    for (it = players_.begin(); it != players_.end(); it++)
        (*it)->gameStarting(this);
    for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
        (*lit_)->gameStart(this);

    // The game continues while there is more than 1 player
    while (players_.size() > 1)
    {
        // Reset variables
        tricksLeft_ = min(defender_->getNumCards(), HAND_SIZE);
        playedCards_.clear();
        playableRanks_.clear();
        refillOrder_.clear();
        // Broadcast start of a round
        for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
            (*lit_)->newRound(attacker_, defender_);
        // Round
        bool successfulDefend = doRound();
        // Broadcast end of round
        for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
            (*lit_)->endRound(successfulDefend);
        // If the defender loses, then they must take all of the played cards.
        if (!successfulDefend)
            // And the attackers can pile on cards
            pileOn();
        else
            // Otherwise they can refill cards
            refillOrder_.push_back(defender_);
        // Refill
        refill();
        // Go out
        removeFinishedPlayers();
        // new defender
        nextDefender(successfulDefend);
    }

    ConstPlayerPtr biscuit;
    // If there is a player left, they're the biscuit
    if (players_.size() == 1)
        biscuit = ConstPlayerPtr(players_[0]);
    // Broadcast the game over with the loser or NULL and the first player out
    for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
        (*lit_)->gameOver(winningPlayer_, biscuit);
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
        for (unsigned i = 0; i < players_.size(); i++)
            hands[i] = deck_.deal(HAND_SIZE);
    } while (!validateHands(hands));

    // Give each player their hand
    for (unsigned i = 0; i < players_.size(); i++)
        players_[i]->addCards(hands[i]);
}

bool Game::validateHands(const vector<vector<Card> >& hands) const
{
    // Some sanity checks
    assert(hands.size() == players_.size());

    // Loop over each hand and see if its no good
    for (unsigned i = 0; i < hands.size(); i++)
    {
        assert(hands[i].size() == HAND_SIZE);
        unsigned red = 0, black = 0;
        unsigned ccnt = 0, scnt = 0, hcnt = 0, dcnt = 0;
        // Loop over each card in the hand and update the count variables
        for (unsigned j = 0; j < hands[i].size(); j++)
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
    assert(tricksLeft_ > 0); // Sanity Check, defender must have at least 1 card

    // Loop invariant: There are still attacking cards to be played
    while (tricksLeft_ > 0)
    {
        // Get the next attacking card from any attacker
        Card attC = getAttackingCard();
        // Did they all pass?
        if (!attC)
            // If so, the defender won!
            break;
        
        // Record the card
        playedCards_.push_back(attC);
        playableRanks_.insert(attC.getNum());
        --tricksLeft_;
        // Broadcast the card
        for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
            (*lit_)->attackingCard(attC);

        // Now record the player in the refill order, if they're already there
        // don't readd them
        if (find(refillOrder_.begin(), refillOrder_.end(), attacker_)
            == refillOrder_.end())
            refillOrder_.push_back(attacker_);

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
        for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
            (*lit_)->defendingCard(defC);
    }

    // If there are no tricks left to play, then the defender has won!
    return true;
}

Card Game::getAttackingCard(bool pileOn)
{
    // Used to tell when all the attackers have passed
    const PlayerPtr initialAttacker = attacker_;

    // while all the attackers haven't passed
    do
    {
        // Get the card from the attacker, could be a pass
		Card attC = Card();
		if (pileOn)
			attC = attacker_->pileOn(playableRanks_);
		else
			attC = attacker_->attack(playableRanks_);

        // If they didn't pass
        if (attC)
            // Then they played a card!
            return attC;
        // If they did pass...
        nextAttacker();
        // Broadcast it
        for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
            (*lit_)->attackerPassed(attacker_);
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
        Card attC = getAttackingCard(true);
        // Did the play a card?
        if (attC)
        {
            // Record it
            playedCards_.push_back(attC);
            --tricksLeft_;
            // Now record the player in the refill order, if they're already there
            // don't readd them
            if (find(refillOrder_.begin(), refillOrder_.end(), attacker_)
                == refillOrder_.end())
                refillOrder_.push_back(attacker_);
            // Broadcast it
            for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
                (*lit_)->piledOnCard(attC);
        }
        // Did they all pass?
        else
            // Then we're done
            break;
    }

    // Finally, the defender has to take all of the played cards.
    defender_->addCards(playedCards_);
    // Broadcast the event
    for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
        (*lit_)->givenCards(defender_, playedCards_);
}

void Game::refill()
{
    for (unsigned i = 0; i < refillOrder_.size(); i++)
    {
        // You refill to HAND_SIZE, but only if there are enough cards.
        int neededCards = min(HAND_SIZE - refillOrder_[i]->getNumCards(),
                              deck_.getNumCards());
        if (neededCards > 0)
        {
            vector<Card> refillCards = deck_.deal(neededCards);
            refillOrder_[i]->addCards(refillCards);
            // Broadcast the drawing
            for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
                (*lit_)->givenCards(refillOrder_[i], neededCards);
        }
    }
}

void Game::removeFinishedPlayers()
{
    unsigned i = 0;
    while (i < players_.size())
    {
        if (players_[i]->getNumCards() == 0)
        {
            const PlayerPtr goingOut = players_[i];
            // Remove that player using random access iterator
            // The next player is now accessed using the same index i
            players_.erase(players_.begin() + i);
            // Check to see if they were before the defender, update the idx
            // if they were
            if (players_.size() != 0)
            {
                defenderIdx_ = (i >= defenderIdx_) ?
                    defenderIdx_ % players_.size()  :
                    defenderIdx_ - 1;
            }
            // Broadcast the player
            for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
                (*lit_)->playedOut(goingOut);
            // Check to see if it was the first player out
            if (!winningPlayer_)
                winningPlayer_ = goingOut;
        }
        else
            i++;
    }
}

void Game::nextDefender(bool successfulDefend)
{
    // If the game is over, don't bother
    if (players_.size() < 2)
        return;
    // If they lost, it skips them
    if (!successfulDefend)
        defenderIdx_ = (defenderIdx_ + 1) % players_.size();
    // Old defender is new attacker
    attackerIdx_ = defenderIdx_;
    // Defender moves one place
    defenderIdx_ = (defenderIdx_ + 1) % players_.size();

    // Update variables
    attacker_ = players_[attackerIdx_];
    defender_ = players_[defenderIdx_];

    // Sanity checks
    assert(find(players_.begin(), players_.end(), attacker_) != players_.end());
    assert(find(players_.begin(), players_.end(), defender_) != players_.end());
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

int Game::getDiscardSize() const
{
    int aliveCards = deck_.getNumCards();
    for (unsigned i = 0; i < players_.size(); i++)
    {
        aliveCards += players_[i]->getNumCards();
    }
    
    return 36 - aliveCards;
}

ConstPlayerPtr Game::getAttacker() const
{
    return attacker_;
}

ConstPlayerPtr Game::getDefender() const
{
    return defender_;
}

const vector<PlayerPtr> Game::getPlayers() const
{
    return players_;
}

const vector<Card>& Game::getPlayedCards() const
{
    return playedCards_;
}
