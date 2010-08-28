#pragma once
#include "NetworkListener.h"

class NetworkPlayer :
    public NetworkListener
{
public:
    NetworkPlayer(GameAgent* agent);
    virtual ~NetworkPlayer();

    // Methods inherited from the player interface
    virtual void gameStarting(GameAgent *agent);
    virtual Card defend(const Card &attC, Card::cardsuit trump);
    virtual Card attack(std::set<int> playableRanks);
    virtual Card pileOn(std::set<int> playableRanks);
    virtual void addCards(const std::vector<Card>& cards);
    virtual int  getNumCards() const;
    virtual std::string getName() const;


private:
    // TODO add a member variable for the hand
    std::string name_;
};
