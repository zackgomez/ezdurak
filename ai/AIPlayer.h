#pragma once
#include "core/Player.h"

class AIPlayer :
    public Player
{
public:
    AIPlayer(const std::string& name);
    virtual ~AIPlayer();

    // Methods Inherited from Player interface
    virtual void gameStarting(GameAgent *agent);
    virtual Card defend(const Card& attackingCard, Card::cardsuit trump);
    virtual Card attack(std::set<int> playableRanks = std::set<int>());
    virtual Card pileOn(std::set<int> playableRanks);
    virtual void addCards(const std::vector<Card>& cards);

protected:
    // Members
    GameAgent *agent_;
    // Helper functions
    void removeCard(const Card& card);
    std::vector<Card> playableCards(std::set<int> playableRanks) const;
    std::vector<Card> defendableCards(const Card& card, Card::cardsuit trump) const;
    void orderCards(std::vector<Card>& cards) const;

    bool isPartner(const Player *p) const;
};
