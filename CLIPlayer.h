#pragma once
#include "Player.h"

class CLIPlayer : public Player
{
public:
    CLIPlayer(const std::string& name, const std::vector<Card>& hand);
    virtual ~CLIPlayer();

    virtual Card defend(const Card& attackingCard, Card::cardsuit trump);
    virtual Card attack(std::set<int> playableRanks = std::set<int>());
    virtual Card pileOn(std::set<int> playableRanks);

    virtual void addCards(const std::vector<Card>& cards);

private:
    CLIPlayer(const CLIPlayer&);
    void operator=(const Player&);
};
