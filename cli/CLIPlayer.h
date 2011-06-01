#pragma once
#include "core/PlayerImpl.h"

/**
 * An implementation of the Player interface, via PlayerImpl that gets input
 * when necessary from the command line.  Should be used in conjunction with
 * CLIListener to give full information about the played game.
 */
class CLIPlayer : public PlayerImpl
{
public:
    CLIPlayer(const std::string& name);
    virtual ~CLIPlayer();

    virtual Card defend(const Card& attackingCard, Card::cardsuit trump);
    virtual Card attack(std::set<int> playableRanks = std::set<int>());
    virtual Card pileOn(std::set<int> playableRanks);
    virtual Card deflect(const Card& attackingCard);

    virtual void addCards(const std::vector<Card>& cards);

private:
    CLIPlayer(const CLIPlayer&);
    void operator=(const Player&);

    void sortHand();
};
