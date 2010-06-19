#pragma once
#include <vector>
#include <string>
#include <set>
#include "card.h"

class GamePlayer
{
public:
    GamePlayer(std::string& name, std::vector<Card>& hand);
    virtual ~GamePlayer();

    virtual Card defend(Card& attackingCard, Card::cardsuit trump);
    virtual Card attack(std::set<int> playableRanks = std::set<int>());

    virtual void addCards(std::vector<Card>& cards);
    int numCards() const;

    void print() const;

protected:
    std::vector<Card> hand_;
    std::string name_;
};

