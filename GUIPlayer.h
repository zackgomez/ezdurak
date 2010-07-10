#pragma once
#include "SynchronizedQueue.h"
#include "Player.h"

class GUIPlayer :
    public Player
{
public:
    GUIPlayer(const std::string &name, SynchronizedQueue<int> &q);
    ~GUIPlayer();

    const std::vector<Card>& getHand() const;
    SynchronizedQueue<int>& getQueue() const;

    virtual void gameStarting(GameAgent *agent);
    virtual Card defend(const Card& attackingCard, Card::cardsuit trump);
    virtual Card attack(std::set<int> playableRanks = std::set<int>());
    virtual Card pileOn(std::set<int> playableRanks);
private:
    SynchronizedQueue<int> &queue_;
};

