#pragma once
#include "Player.h"

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
    // Helper functions
    std::vector<Card> playableCards(std::set<int> playableRanks);
    std::vector<Card> defendableCards(const Card& card, Card::cardsuit trump);
    void removeCard(const Card& card);
};
