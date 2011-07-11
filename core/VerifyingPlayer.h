#pragma once
#include "Player.h"
#include <vector>
#include <string>
#include "Card.h"
#include <cassert>

/**
 * This player is intended to wrap another play and make sure that all of their
 * players are valid.  It checks to see that played cards are indeed in that 
 * players hand as well as valid for the hand/trick being played.  All checks
 * are done inside of assertions so will be compiled out with NDEBUG.
 * Most methods are simply pass throughs to the wrapped player instance.
 */
class VerifyingPlayer :
    public Player
{
public:
    /** 
     * Creates a VerifyingPlayer from a given player.  The verifying player
     * takes ownership of the passed in player object.
     */
    VerifyingPlayer(Player *p) : player_(p) {}
    virtual ~VerifyingPlayer() { delete player_; }

    virtual void gameStarting(GameAgent *agent) { player_->gameStarting(agent); }

    virtual Card defend(const Card& attackingCard, Card::cardsuit trump)
    {
        Card c = player_->defend(attackingCard, trump);
        // check
        assert(hand_.find(c) != hand_.end());
        assert(c.beats(attackingCard, trump));
        // remove card
        hand_.erase(hand.find(c));
        return c;
    }
    virtual Card attack(std::set<int> playableRanks)
    {
        Card c = player_->attack(playableRanks);
        assert(hand_.find(c) != hand_.end());
        assert(playableRanks.find(c.getNum()) != playableRanks.end());
        // remove card
        hand_.erase(hand.find(c));
        return c;
    }
    virtual Card pileOn(std::set<int> playableRanks)
    {
        Card c = player_->pileOn(playableRanks);
        assert(hand_.find(c) != hand_.end());
        assert(playableRanks.find(c.getNum()) != playableRanks.end());
        // remove card
        hand_.erase(hand.find(c));
        return c;
    }

    virtual void addCards(const std::vector<Card>& cards)
    {
        player_->addCards(cards);
        hand_.insert(cards.begin(), cards.end());
    }
    virtual unsigned getNumCards() const
    {
        assert(player_->getNumCards() == hand_.size());
        return player_->getNumCards();
    }
    virtual std::string getName() const { return player_->getName(); }
    virtual std::string getID() const { return player_->getID(); }

protected:
    /// The Player's hand
    std::vector<Card> hand_;
    /// The Player being verified
    Player *player_;
};

