#pragma once
#include "NetworkListener.h"
#include "core/Player.h"
#include <set>

class NetworkPlayer :
    public NetworkListener,
    public Player
{
public:
    NetworkPlayer(GameAgent* agent);
    virtual ~NetworkPlayer();

    // Overriden from NetworkListener
    bool getConnection(const std::string &port);

    // Methods inherited from the Player interface
    virtual void gameStarting(GameAgent *agent);
    virtual Card defend(const Card &attC, Card::cardsuit trump);
    virtual Card attack(std::set<int> playableRanks);
    virtual Card pileOn(std::set<int> playableRanks);
    virtual void addCards(const std::vector<Card>& cards);
    virtual int  getNumCards() const;
    virtual std::string getName() const;


private:
    std::set<Card> hand_;
    std::string name_;
};
