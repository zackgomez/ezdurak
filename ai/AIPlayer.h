#pragma once
#include "core/PlayerImpl.h"

/**
 * This is a simple, but not stupid AI for durak.  It implements the
 * Player interface via the PlayerImpl helper class.  It currently implements
 * a few strategies and useful helper functions.  Any "better" AI would find
 * this a very useful base class.
 *
 * Current behaviors:
 *  - Has a rudimentary understanding of "partners" and will never attack a
 *    partner.
 *  - It will however pileOn to a parter once they have already lost.
 *  - Never piles on trump
 *  - Always attacks/defends with the lowest value card (trump are highest).
 */
class AIPlayer :
    public PlayerImpl
{
public:
    AIPlayer(const std::string& name);
    virtual ~AIPlayer();

    // Methods Inherited from Player interface
    virtual void gameStarting(GameAgent *agent);
    virtual Card defend(const Card& attackingCard, Card::cardsuit trump);
    virtual Card attack(std::set<int> playableRanks = std::set<int>());
    virtual Card deflect(const Card& attackingCard, ConstPlayerPtr newAttacker,
            constPlayerPtr newDefender);
    virtual Card pileOn(std::set<int> playableRanks);

protected:
    // Helper functions
    /// Removes the passed card from the hand
    void removeCard(const Card& card);
    /// Returns a list of playable cards given the playableRanks
    std::vector<Card> playableCards(std::set<int> playableRanks) const;
    /// Returns a list of cards that can defend the passed card, given trump
    std::vector<Card> defendableCards(const Card& card, Card::cardsuit trump) const;
    /// Sorts the passed cards, in place, by rank, then trump
    void orderCards(std::vector<Card>& cards) const;

    /// Returns true if the passed player is this player's "partner"
    bool isPartner(ConstPlayerPtr p) const;
};
