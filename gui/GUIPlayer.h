#pragma once
#include "SynchronizedQueue.h"
#include "core/PlayerImpl.h"

class GUIPlayer :
    public PlayerImpl
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
    virtual void addCards(const std::vector<Card>& cards);
private:
    void sortHand();
    SynchronizedQueue<int> &queue_;
    GameAgent *agent_;
};

