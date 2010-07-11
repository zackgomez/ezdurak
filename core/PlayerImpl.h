#pragma once
#include "Player.h"
#include <vector>
#include <string>
#include "Card.h"

class PlayerImpl :
    public Player
{
public:
    PlayerImpl(const std::string &name);
    virtual ~PlayerImpl();

    /**
     * Called at the start of the game.
     */
    virtual void gameStarting(GameAgent *agent) = 0;

    virtual Card defend(const Card& attackingCard, Card::cardsuit trump) = 0;
    virtual Card attack(std::set<int> playableRanks) = 0;
    virtual Card pileOn(std::set<int> playableRanks) = 0;

    virtual void addCards(const std::vector<Card>& cards);
    virtual int getNumCards() const;
    virtual std::string getName() const;

    void print() const;

protected:
    std::vector<Card> hand_;
    std::string name_;
};

